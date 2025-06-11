#include "scenes/ShadowMap_sc.hpp"

#include "graphics/core/Shader.hpp"
#include "graphics/core/Texture.hpp"
#include "graphics/core/Sprite.hpp"

#include "objects/Voxel.hpp"

#include "graphics/glsl/GLSLHelper.hpp"
#include "graphics/core/VertexStructures.hpp"
#include "core/Logger.hpp"



ShadowMap_sc::ShadowMap_sc() = default;
ShadowMap_sc::~ShadowMap_sc() = default;

std::vector<glm::vec3> getInstanceOffsets(int side_size) {
    std::vector<glm::vec3> offsets;
    offsets.reserve(side_size * side_size);

    for (size_t x = 0; x < side_size; x++)
    {
        for (size_t z = 0; z < side_size; z++)
        {
            offsets.emplace_back(glm::vec3(x * 15.0f, 0.0f, z * 15.0f));
        }
    }
    return offsets;
}

void ShadowMap_sc::init(Engine& engine, Window& window) 
{
    window_ = &window;
    time_ = &engine.getTime();

    // FPS TIMER SET UP
    core::Timer timer;
    timer.finish_time = 1.0f;
    timer.time_out = [this]() { 
        std::cout << "FPS: " + std::to_string(int(1.0f / time_->getDeltaTime())) << std::endl;
    };
    engine.getTime().addTimer(std::move(timer));

    // OPENGL SET UP
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //
    // glDisable(GL_BLEND);
    // glDisable(GL_STENCIL_TEST);
    
    // GLFW SET UP
    glfwSwapInterval(vsync);

    // CAMERA SET UP
    CameraParams c_params;
    c_params.z_far = 4'000.0f;
    camera_ = makeU<Camera>(window, c_params);
    camera_->setTransform(glm::vec3(0.0f, 1.0f, 10.0f));

    // BOX SET UP
    Image image;
    image.load("res/images/Prototype_Grid_Gray_03-512x512.png");
    //
    TextureParams t_params;
    t_params.min_filter = GL_LINEAR_MIPMAP_LINEAR;
    t_params.mag_filter = GL_LINEAR_MIPMAP_LINEAR;
    box_texture_ = makeU<Texture>();
    Texture::activeUnit(1);
    box_texture_->create(image, t_params);

    box_ = makeU<Voxel>(glm::vec3(0.0f, 1.0f, 0.0f));
    box_->transform.rotate(glm::vec3(25.0f, 0.0f, 25.0f));

    // SHADER SET UP
    sh_main_ = makeU<Shader>();
    sh_main_->create("res/shaders/shadow_map/main.vert", "res/shaders/shadow_map/main.frag");
    sh_main_->use();
    sh_main_->uniform1i(DIFFUSE, box_texture_->getUnitId());
    sh_main_->uniform1i(SHADOW_MAP, 2);
    sh_main_->uniform1i("u_shadow_on", shadow_on_);
    //
    sh_depth_ = makeU<Shader>();
    sh_depth_->create("res/shaders/shadow_map/depth.vert", "res/shaders/shadow_map/depth.frag");
    sh_depth_->use();
    sh_depth_->uniform1i(TEXTURE, 2);    
    //
    sh_simple_ = makeU<Shader>();
    sh_simple_->create("res/shaders/shadow_map/simple.vert", "res/shaders/shadow_map/simple.frag");     

    // FLOOR SET UP
    floor_ = makeU<Sprite>();
    floor_->transform.rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    floor_->transform.setScale(glm::vec3(floor_size_));
    floor_->setRegion(0, 0, floor_size_, floor_size_);
    floor_->generate();

    // LIGHT SET UP
    light_pos_ = glm::vec3(0.0f, 5.0f, 0.0f);
    light_dir_ = glm::vec3(-0.1f, -1.0f, 0.01f);
    light_distance = 25.0f;
    float near_plane = 1.0f, far_plane = light_distance;
    light_projection = glm::ortho(-light_distance, light_distance,-light_distance, light_distance, near_plane, far_plane);

    // DEPTH FBO SET UP
    SHADOW_WIDTH = SHADOW_K_4;
    SHADOW_HEIGHT = SHADOW_WIDTH;
    Texture::activeUnit(2);
    fbo_.create(DIRECTION_DEPTH_PARAMS, SHADOW_WIDTH, SHADOW_HEIGHT, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT);

    // MODEL SET UP
    model_ = makeU<modload::Model>();
    Texture::activeUnit(0);
    //model_->create("D:/Mine(D)/Programming/C++/Models/grass/Low/Low Grass.fbx");
    model_->create("res/models/backpack/backpack.obj");
    
    // CREATING INSTANCES
    std::vector<glm::vec3> temp_offsets = getInstanceOffsets(side_size_);
    auto& temp_meshes = model_->getMeshes();
    for (size_t i = 0; i < temp_meshes.size(); i++)
    {
        temp_meshes[i].mesh.bind();
        temp_meshes[i].mesh.setBuffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * temp_offsets.size(), temp_offsets.data(), GL_STATIC_DRAW);
        temp_meshes[i].mesh.setAttrib(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0, true);
    }

}

void ShadowMap_sc::input(InputManager& input, float delta) 
{
    if (input.justPressed(GLFW_KEY_TAB))
    {
        window_->toggleCursor();
    }

    if (input.isCursorLocked())
    {
        camera_->process3DMouseRotation(input.getDeltaX(), input.getDeltaY());
        if (input.pressed(GLFW_KEY_W))
        {
            camera_->translate(camera_->getFront() * camera_speed_ * delta);
        }
        if (input.pressed(GLFW_KEY_S))
        {
            camera_->translate(-camera_->getFront() * camera_speed_ * delta);
        }        
        if (input.pressed(GLFW_KEY_A))
        {
            camera_->translate(-camera_->getRight() * camera_speed_ * delta);
        }
        if (input.pressed(GLFW_KEY_D))
        {
            camera_->translate(camera_->getRight() * camera_speed_ * delta);
        }


        if (input.justPressed(GLFW_KEY_LEFT_SHIFT))
        {
            camera_speed_ *= 2.0f;
        }
        if (input.justReleased(GLFW_KEY_LEFT_SHIFT))
        {
            camera_speed_ /= 2.0f;
        }
    }

    if (input.getScrollDeltaY() > 0.0f){
        camera_->toZoom(0.8f, 0.0f, 20.0f);
    }
    else if (input.getScrollDeltaY() < 0.0f){
        camera_->toZoom(1.2f, 0.0f, 20.0f);
    }

    if (input.justPressed(GLFW_KEY_F)) {
        vsync = !vsync;
        glfwSwapInterval(vsync);
    }    

    if (input.justPressed(GLFW_KEY_Q)) {
        shadow_on_ = !shadow_on_;
        sh_main_->use();
        sh_main_->uniform1i("u_shadow_on", shadow_on_);
    }
    
}

void ShadowMap_sc::update(float delta) 
{
    rotation += 1.0f * delta;
}

void ShadowMap_sc::draw() 
{
    // glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (shadow_on_)
    {
        glm::vec3 camera_offset_ = glm::vec3(camera_->getPos().x, 0.0f, camera_->getPos().z);
        glm::mat4 lightView = glm::lookAt(light_pos_ + camera_offset_, light_pos_ + light_dir_ + camera_offset_, glm::vec3( 0.0f, 1.0f, 0.0f));

        light_space_matrix = light_projection * lightView;

        fbo_.bind();
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glCullFace(GL_FRONT);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        sh_simple_->use();
        sh_simple_->uniformMat4("u_light_space_matrix", light_space_matrix);

        renderScene(*sh_simple_.get());
    }
    fbo_.unbind();
    glViewport(0, 0, window_->getWidth(), window_->getHeight());
    glCullFace(GL_BACK);
    glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sh_main_->use();
    if (shadow_on_) 
    {
        sh_main_->uniformMat4("u_light_space", light_space_matrix);
        sh_main_->uniform3f(VIEW_POS, camera_->getPos()); 
        sh_main_->uniform3f(LIGHT_DIR, light_dir_); 
    }
    sh_main_->uniformMat4(PROJ_VIEW, camera_->getProjection() * camera_->getView());

    renderScene(*sh_main_.get(), false);

}


void ShadowMap_sc::renderScene(Shader& shader, bool is_depth) 
{
    // BOX DRAWING
    if (!is_depth)
        shader.uniform1i(DIFFUSE, box_texture_->getUnitId());
    shader.uniformMat4(MODEL, box_->transform.getModel());
    if (!is_depth)
    {
        shader.uniformMat3(NORMAL, glm::transpose(glm::inverse(glm::mat3(box_->transform.getModel()))));
    }
    box_->draw();

    // FLOOR DRAWING
    shader.uniformMat4(MODEL, floor_->transform.getModel());
    if (!is_depth) 
    {
        shader.uniformMat3(NORMAL, glm::transpose(glm::inverse(glm::mat3(floor_->transform.getModel()))));
    }
    floor_->draw();

    // MODEL DRAWING
    if (!is_depth)
        shader.uniform1i(DIFFUSE, 0);
    glm::mat4 md = glm::mat4(1.0f);
    md = glm::translate(md, glm::vec3(2.0f, 3.0f, 0.0f));
    md = glm::rotate(md, glm::radians(rotation), glm::vec3(0.0f ,1.0f, 0.0f));
    md = glm::scale(md, glm::vec3(0.4f));
    shader.uniformMat4(MODEL, md);
    if (!is_depth) 
    {
        shader.uniformMat3(NORMAL, glm::transpose(glm::inverse(glm::mat3(md))));
    }
    auto& meshes_ = model_->getMeshes();
    auto& materials_ = model_->getMaterials();
    auto& textures_ = model_->getAllTextures();
    int current_material_index = -1;
    glActiveTexture(GL_TEXTURE0);
    for (size_t i = 0; i < meshes_.size(); i++)
    {
        int mat_idx = meshes_[i].material_index;
        if (current_material_index != mat_idx) 
        {
            textures_[materials_[mat_idx].diffuse].bind();
            current_material_index = mat_idx;
        }
        meshes_[i].mesh.drawInstances(side_size_ * side_size_);
    }  
}

void ShadowMap_sc::onClose()
{
    #ifdef MEMORY_DEBUG
        print_Alloc_Memory_Kilobyte();
        print_Dealloc_Memory_Kilobyte();
        print_Usage_Memory_Kilobyte();
    #endif    
}