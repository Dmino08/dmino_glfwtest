#include "scenes/PointShadows_sc.hpp"

#include "graphics/core/Shader.hpp"
#include "graphics/core/Texture.hpp"


#include "graphics/glsl/GLSLHelper.hpp"
#include "graphics/core/VertexStructures.hpp"
#include "core/Logger.hpp"
#include "core/MemoryTracker.hpp"



void DepthPointLight::setUpMatrix()
{
    shad_transforms[0] = (shad_proj * glm::lookAt(pos, pos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0, -1.0,  0.0)));
    shad_transforms[1] = (shad_proj * glm::lookAt(pos, pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0,  0.0)));
    shad_transforms[2] = (shad_proj * glm::lookAt(pos, pos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0,  0.0,  1.0)));
    shad_transforms[3] = (shad_proj * glm::lookAt(pos, pos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0,  0.0, -1.0)));
    shad_transforms[4] = (shad_proj * glm::lookAt(pos, pos + glm::vec3( 0.0,0.0, 1.0),  glm::vec3(0.0, -1.0,  0.0)));
    shad_transforms[5] = (shad_proj * glm::lookAt(pos, pos + glm::vec3( 0.0,0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));
}

PointShadows_sc::PointShadows_sc() : sides_()
{}

PointShadows_sc::~PointShadows_sc() = default;


void PointShadows_sc::init(Engine& engine, Window& window)
{
    window_ = &window;
    time_ = &engine.getTime();
    engine_ = &engine;

    // FPS TIMER SET UP
    // core::Timer timer;
    // timer.time_out = [this]() { 
    //     std::cout << "FPS: " + std::to_string(int(1.0f / time_->getDeltaTime())) << '\n';
    // };
    // engine.getTime().addTimer(std::move(timer));

    // OPENGL SET UP
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    
    // GLFW SET UP
    glfwSwapInterval(1);

        // CAMERA SET UP
    CameraParams c_params;
    c_params.z_far = 4'000.0f;
    camera_ = makeU<Camera>(window, c_params);
    camera_->setTransform(glm::vec3(0.0f, 1.0f, 10.0f));

    // SHADER SET UP
    sh_main_ = makeU<Shader>();
    sh_main_->create("res/shaders/point_shadows/main.vert", "res/shaders/point_shadows/main.frag");
    //
    sh_shadow_ = makeU<Shader>();
    sh_shadow_->create( "res/shaders/point_shadows/shadow.vert",
                        "res/shaders/point_shadows/shadow.frag",
                        "res/shaders/point_shadows/shadow.geom");


    // JUST IMAGE
    Image::flipLoad(true);
    Image image;

    // JUST TEXTURE PARAMS
    TextureParams t_params;
    t_params.min_filter = GL_LINEAR_MIPMAP_LINEAR;

    // SIDES SET UP
    image.load("res/images/Prototype_Grid_Gray_03-512x512.png");
    //
    simple_texture_ = makeU<Texture>();
    Texture::activeUnit(Texture::getFreeUnit());
    simple_texture_->create(image, t_params);
    simple_texture_->bind();
    //
    Sprite side;
    side.transform.setScale(glm::vec3(side_size_));
    side.setRegion(0, 0, side_size_, side_size_);
    //
    sides_.fill(side);
    // FLOOR
    sides_[0].transform.setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    sides_[0].transform.setPosition(glm::vec3(0.0f, -side_size_ / 2, 0.0f));
    sides_[0].generate();
    // CEILING
    sides_[1].transform.setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
    sides_[1].transform.setPosition(glm::vec3(0.0f, side_size_ / 2, 0.0f));
    sides_[1].generate();
    // RIGHT
    sides_[2].transform.setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    sides_[2].transform.setPosition(glm::vec3(side_size_ / 2, 0.0f, 0.0f));
    sides_[2].generate();
    // LEFT
    sides_[3].transform.setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    sides_[3].transform.setPosition(glm::vec3(-side_size_ / 2, 0.0f, 0.0f));
    sides_[3].generate();
    // FAR
    sides_[4].transform.setRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    sides_[4].transform.setPosition(glm::vec3(0.0f, 0.0f, side_size_ / 2));
    sides_[4].generate();
    // NEAR
    sides_[5].transform.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sides_[5].transform.setPosition(glm::vec3(0.0f, 0.0f, -side_size_ / 2));
    sides_[5].generate();

    // BOXES SET UP
    image.load("res/images/container2.png");
    box_texture_ = makeU<Texture>();
    Texture::activeUnit(Texture::getFreeUnit());
    box_texture_->create(image, t_params);
    box_texture_->bind();
    //
    boxes_[0] = Voxel(glm::vec3(5.0f, 2.0f, 0.0f));
    boxes_[1] = Voxel(glm::vec3(-5.0f, -2.0f, 2.0f));
    boxes_[2] = Voxel(glm::vec3(5.0f, 0.0f, -5.0f));
    boxes_[3] = Voxel(glm::vec3(2.0f, 0.0f, 5.0f));
    boxes_[4] = Voxel(glm::vec3(2.0f, 0.0f, 5.0f));
    boxes_[5] = Voxel(glm::vec3(7.0f, -5.0f, 5.0f));
    boxes_[6] = Voxel(glm::vec3(1.0f, 0.0f, -5.0f));
    boxes_[7] = Voxel(glm::vec3(0.0f, 7.0f, 5.0f));
    boxes_[8] = Voxel(glm::vec3(2.0f, -6.0f, 5.0f));
    boxes_[9] = Voxel(glm::vec3(0.0f, 1.0f, 0.0f));

    // MODEL SET UP
    model_ = makeU<modload::Model>();
    model_->create("res/models/backpack/backpack.obj");

    // FBO SET UP
    SHADOW_W_H = SHADOW_K_1;
    fbo_.create(POINT_DEPTH_PARAMS, SHADOW_W_H, SHADOW_W_H, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT);

    // LIGHT SET UP
    dpl_.far_plane = 25.0f;
    dpl_.shad_proj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, dpl_.far_plane);
    dpl_.pos = glm::vec3(0.0f);
    dpl_.setUpMatrix();

}

void PointShadows_sc::input(InputManager& input, float delta)
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

        if (input.pressed(KEY_UP))
        {
            dpl_.pos.z += 1.0 * delta;
        }
        if (input.pressed(KEY_DOWN))
        {
            dpl_.pos.z -= 1.0 * delta;
        }
        if (input.pressed(KEY_LEFT))
        {
            dpl_.pos.x -= 1.0 * delta;
        }
        if (input.pressed(KEY_RIGHT))
        {
            dpl_.pos.x += 1.0 * delta;
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


    if (input.justPressed(GLFW_KEY_1)) {
        engine_->events.addEvent([engine_ = engine_](){
            engine_->attachSceneToWindow("1", "1");
            std::cout << "Attached 1\n"; 
        });
    }

}

void PointShadows_sc::update(float delta) {
    fps_++;
    if (time_->each(1.0f)) {
        std::cout << "FPS: " << fps_ << '\n';
        fps_ = 0;
    }
    
    rotation_ += 15.0 * delta;
    
}

void PointShadows_sc::draw() {   
    fbo_.bind();
    glCullFace(GL_FRONT);
    glViewport(0, 0, SHADOW_W_H, SHADOW_W_H);
    glClear(GL_DEPTH_BUFFER_BIT);

    // SHADOW SETUP
    sh_shadow_->use();
    sh_shadow_->uniform3f(LIGHT_POS, dpl_.pos);
    sh_shadow_->uniform1f("u_far_plane", dpl_.far_plane);
    for (size_t i = 0; i < 6; i++)
    {
        sh_shadow_->uniformMat4("u_shadow_matrices[" + std::to_string(i) + "]", dpl_.shad_transforms[i]);
    }

    renderScene(*sh_shadow_, true);    

    fbo_.unbind();
    glCullFace(GL_BACK);
    glViewport(0, 0, window_->getWidth(), window_->getHeight());
    glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    


    // MAIN SETUP
    sh_main_->use();
    sh_main_->uniformMat4(PROJ_VIEW, camera_->getProjView());
    sh_main_->uniform3f(LIGHT_POS, dpl_.pos);
    sh_main_->uniform3f(VIEW_POS, camera_->getPos());
    sh_main_->uniform1f("u_far_plane", dpl_.far_plane);
    sh_main_->uniform1i(SHADOW_MAP, fbo_.texture.getUnitId());


    renderScene(*sh_main_, false);
    
}

void PointShadows_sc::onClose()
{
    print_Alloc_Memory_Kilobyte();
    print_Dealloc_Memory_Kilobyte();
    print_Usage_Memory_Kilobyte();
}

void PointShadows_sc::renderScene(Shader& shader, bool is_depth)
{
    if (!is_depth)
    {
        shader.uniform1i(DIFFUSE, simple_texture_->getUnitId());
    }
    for (auto& side : sides_)
    { 
        shader.uniformMat4(MODEL, side.transform.getModel());
        if (!is_depth)
        {
            shader.uniformMat3(NORMAL, glm::transpose(glm::inverse(side.transform.getModel())));
        }
        side.draw();        
    }
    if (!is_depth) 
    {
        shader.uniform1i(DIFFUSE, box_texture_->getUnitId());
    }
    for (auto& box : boxes_)
    {
        box.transform.setRotation(glm::vec3(rotation_));
        shader.uniformMat4(MODEL, box.transform.getModel());
        if (!is_depth) 
        {
            shader.uniformMat3(NORMAL, glm::transpose(glm::inverse(box.transform.getModel())));
        }
        box.draw();  
    }   
    
    glm::mat4 md = glm::mat4(1.0f);
    md = glm::translate(md, glm::vec3(-4.0f, 2.0f, 1.0f));
    md = glm::rotate(md, glm::radians(rotation_), glm::vec3(1.0f, 1.0f, 1.0f));
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
        if (current_material_index != mat_idx) {
            if (!is_depth)
                shader.uniform1i(DIFFUSE, textures_[materials_[mat_idx].diffuse].getUnitId());
            else
                shader.uniform1i(TEXTURE, textures_[materials_[mat_idx].diffuse].getUnitId());
        }
        meshes_[i].mesh.draw();
    } 
}