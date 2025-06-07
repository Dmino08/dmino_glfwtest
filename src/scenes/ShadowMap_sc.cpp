#include "scenes/ShadowMap_sc.hpp"

#include "graphics/core/Shader.hpp"
#include "graphics/core/Texture.hpp"
#include "graphics/core/Sprite.hpp"

#include "objects/Voxel.hpp"

#include "graphics/glsl/GLSLHelper.hpp"


ShadowMap_sc::ShadowMap_sc() = default;
ShadowMap_sc::~ShadowMap_sc() = default;

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

    // SHADER SET UP
    sh_main_ = makeU<Shader>();
    sh_main_->create("res/shaders/shadow_map/3d.vert", "res/shaders/shadow_map/3d.frag");
    
    // BOX SET UP
    Image image;
    image.load("res/images/Prototype_Grid_Gray_03-512x512.png");
    //
    TextureParams t_params;
    t_params.min_filter = GL_LINEAR_MIPMAP_LINEAR;
    box_texture_ = makeU<Texture>();
    box_texture_->create(image, t_params);
    box_texture_->activeUnit(0);
    //
    box_ = makeU<Voxel>(glm::vec3(0.0f, 1.0f, 0.0f));

    // FLOOR SET UP
    floor_ = makeU<Sprite>();
    floor_->transform.rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    floor_->transform.setScale(glm::vec3(floor_size_));
    floor_->setRegion(0, 0, floor_size_, floor_size_);
    floor_->generate();
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
    }
    
}

void ShadowMap_sc::draw() 
{
    glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sh_main_->use();
    setMatrices(*sh_main_, camera_->getProjection(), camera_->getView(), box_->transform.getModel());
    box_->draw();

    setMatrices(*sh_main_, camera_->getProjection(), camera_->getView(), floor_->transform.getModel());
    floor_->draw();
}