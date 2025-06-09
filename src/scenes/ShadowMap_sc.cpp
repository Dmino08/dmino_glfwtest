#include "scenes/ShadowMap_sc.hpp"

#include "graphics/core/Shader.hpp"
#include "graphics/core/Texture.hpp"
#include "graphics/core/Sprite.hpp"

#include "objects/Voxel.hpp"

#include "graphics/glsl/GLSLHelper.hpp"
#include "graphics/core/VertexStructures.hpp"


ShadowMap_sc::ShadowMap_sc() = default;
ShadowMap_sc::~ShadowMap_sc() = default;

std::vector<glm::vec3> getInstanceOffsets(int side_size) {
    std::vector<glm::vec3> offsets;
    offsets.reserve(side_size * side_size);

    for (size_t x = 0; x < side_size; x++)
    {
        for (size_t z = 0; z < side_size; z++)
        {
            offsets.push_back(glm::vec3(x * 15.0f, 0.0f, z * 15.0f));
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
    timer.time_out = [this]() { 
        std::string fps = "FPS: " + std::to_string(int(1.0f / time_->getDeltaTime()));
        window_->setTitle(fps);
    };
    engine.getTime().addTimer(std::move(timer));

    // OPENGL SET UP
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);
    
    // GLFW SET UP
    glfwSwapInterval(0);

    // CAMERA SET UP
    CameraParams c_params;
    c_params.z_far = 15'000.0f;
    camera_ = makeU<Camera>(window, c_params);
    camera_->setTransform(glm::vec3(0.0f, 1.0f, 10.0f));

    // BOX SET UP
    Image image;
    image.load("res/images/Prototype_Grid_Gray_03-512x512.png");
    //
    TextureParams t_params;
    t_params.min_filter = GL_LINEAR_MIPMAP_LINEAR;
    box_texture_ = makeU<Texture>();
    box_texture_->create(image, t_params);
    box_texture_->activeUnit(1);
    //
    box_ = makeU<Voxel>(glm::vec3(0.0f, 1.0f, 0.0f));
    box_->transform.rotate(glm::vec3(25.0f, 0.0f, 25.0f));


    // SHADER SET UP
    sh_main_ = makeU<Shader>();
    sh_main_->create("res/shaders/shadow_map/main.vert", "res/shaders/shadow_map/main.frag");
    sh_main_->use();
    sh_main_->uniform1i(DIFFUSE, box_texture_->getUnitId());
    sh_main_->uniform1i(SHADOW_MAP, 2);
    sh_main_->uniform1i("u_shadow_on", shadow_on_);
    // sh_main_->uniform3f(LIGHT_POS, glm::vec3(-2.0f, 4.0f,-1.0f)); // Point implementation
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

    // DEPTH FBO SET UP
    glGenFramebuffers(1, &depth_fbo_);

    // DEPTH MAP SET UP
    SHADOW_WIDTH = SHADOW_K_4;
    SHADOW_HEIGHT = SHADOW_WIDTH;
    glActiveTexture(GL_TEXTURE2);
    //
    glGenTextures(1, &depth_map_);
    glBindTexture(GL_TEXTURE_2D, depth_map_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
        SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[4] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    // DEPTH ATTACHMENT
    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
        depth_map_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // DEPTH MESH SET UP
    depth_mesh_ = makeU<Mesh>();
    std::vector<ScreenVertex> vertices_;
    if (vertices_.empty())
    {
        vertices_ = {
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f}, {0.0f, 0.0f}},
            {{1.0f, -1.0f},  {1.0f, 0.0f}},
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{1.0f, -1.0f},  {1.0f, 0.0f}},
            {{1.0f,  1.0f},  {1.0f, 1.0f}}
        };
    }    
    depth_mesh_->create(vertices_.size(), 0);
    depth_mesh_->bind();
    depth_mesh_->setBuffer(GL_ARRAY_BUFFER, vertices_.size() * sizeof(ScreenVertex), vertices_.data(), GL_STATIC_DRAW);
    depth_mesh_->setAttrib(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), reinterpret_cast<void*>(offsetof(ScreenVertex, position)));
    depth_mesh_->setAttrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), reinterpret_cast<void*>(offsetof(ScreenVertex, uv_coord)));
    depth_mesh_->unbind();

    // MODEL SET UP
    model_ = makeU<modload::Model>();
    glActiveTexture(GL_TEXTURE0);
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
    glClearColor(0.5f, 0.7f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 lightSpaceMatrix;
    if (shadow_on_) {
        float near_plane = 1.0f, far_plane = light_distance;
        glm::mat4 lightProjection = glm::ortho(-light_distance, light_distance,-light_distance, light_distance, near_plane, far_plane);
        glm::vec3 camera_offset_ = glm::vec3(camera_->getPos().x, 0.0f, camera_->getPos().z);
        glm::mat4 lightView = glm::lookAt(light_pos_ + camera_offset_, light_pos_ + light_dir_ + camera_offset_, glm::vec3( 0.0f, 1.0f, 0.0f));

        lightSpaceMatrix = lightProjection * lightView;

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        // glCullFace(GL_FRONT);
        glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo_);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        sh_simple_->use();
        sh_simple_->uniformMatrix("u_light_space_matrix", lightSpaceMatrix);

        renderScene(*sh_simple_.get());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glCullFace(GL_BACK);
    glViewport(0, 0, window_->getWidth(), window_->getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sh_main_->use();
    if (shadow_on_) 
    {
        sh_main_->uniformMatrix("u_light_space", lightSpaceMatrix);
        sh_main_->uniform3f(VIEW_POS, camera_->getPos()); 
        sh_main_->uniform3f(LIGHT_DIR, light_dir_); 
    }
    sh_main_->uniformMatrix(PROJECTION, camera_->getProjection());
    sh_main_->uniformMatrix(VIEW, camera_->getView());

    renderScene(*sh_main_.get(), false);


}


void ShadowMap_sc::renderScene(Shader& shader, bool is_depth) 
{
    if (!is_depth)
        shader.uniform1i(DIFFUSE, box_texture_->getUnitId());

    shader.uniformMatrix(MODEL, box_->transform.getModel());
    box_->draw();

    shader.uniformMatrix(MODEL, floor_->transform.getModel());
    floor_->draw();

    if (!is_depth)
        shader.uniform1i(DIFFUSE, 0);
    
    glm::mat4 md = glm::mat4(1.0f);
    md = glm::translate(md, glm::vec3(2.0f, 3.0f, 0.0f));
    md = glm::rotate(md, glm::radians(rotation), glm::vec3(0.0f ,1.0f, 0.0f));
    md = glm::scale(md, glm::vec3(0.4f));
    shader.uniformMatrix(MODEL, md);
    auto& meshes_ = model_->getMeshes();
    auto& materials_ = model_->getMaterials();
    auto& textures_ = model_->getAllTextures();
    for (size_t i = 0; i < meshes_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0);
        textures_[materials_[meshes_[i].material_index].diffuse].bind();

        meshes_[i].mesh.drawInstances(side_size_ * side_size_);
    }  
}