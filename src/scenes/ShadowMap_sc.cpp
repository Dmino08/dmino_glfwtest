#include "scenes/ShadowMap_sc.hpp"

#include "graphics/core/Shader.hpp"
#include "graphics/core/Texture.hpp"
#include "graphics/core/Sprite.hpp"

#include "objects/Voxel.hpp"

#include "graphics/glsl/GLSLHelper.hpp"
#include "graphics/core/VertexStructures.hpp"


ShadowMap_sc::ShadowMap_sc() = default;
ShadowMap_sc::~ShadowMap_sc() = default;

constexpr uint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

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
    sh_main_->use();
    sh_main_->uniform1i(TEXTURE, 0);
    //
    sh_depth_ = makeU<Shader>();
    sh_depth_->create("res/shaders/shadow_map/depth.vert", "res/shaders/shadow_map/depth.frag");
    sh_depth_->use();
    sh_depth_->uniform1i(TEXTURE, 1);    
    //
    sh_simple_ = makeU<Shader>();
    sh_simple_->create("res/shaders/shadow_map/simple.vert", "res/shaders/shadow_map/simple.frag");     

    
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

    // DEPTH FBO SET UP
    glGenFramebuffers(1, &depth_fbo_);

    // DEPTH MAP SET UP
    glActiveTexture(GL_TEXTURE1);
    //
    glGenTextures(1, &depth_map_);
    glBindTexture(GL_TEXTURE_2D, depth_map_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
        SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f,-10.0f, 10.0f,near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f,-1.0f),glm::vec3( 0.0f, 0.0f, 0.0f),glm::vec3( 0.0f, 1.0f, 0.0f));

    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo_);
    glClear(GL_DEPTH_BUFFER_BIT);

    sh_simple_->use();
    sh_simple_->uniformMatrix("u_light_space_matrix", lightSpaceMatrix);

    sh_simple_->uniformMatrix(MODEL, box_->transform.getModel());
    box_->draw();

    sh_simple_->uniformMatrix(MODEL, floor_->transform.getModel());
    floor_->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_->getWidth(), window_->getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sh_depth_->use();
    depth_mesh_->draw();

}