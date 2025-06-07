#include "scenes/ShadowMap_sc.hpp"

#include "window/Window.hpp"
#include "window/Camera.hpp"
#include "window/InputManager.hpp"

#include "graphics/core/Shader.hpp"
#include "graphics/core/Image.hpp"
#include "graphics/core/Texture.hpp"
#include "graphics/core/Sprite.hpp"

#include "objects/Voxel.hpp"

constexpr uint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

void ShadowMap_sc::init(Window& window) {
    window_ = &window;
    input_ = &window.getInput();

    // CAMERA CREATING
    CameraParams c_params_;
    c_params_.z_far = 15'000.0f;
    camera_ = makeU<Camera>(window, c_params_);

    // SHADER CREATING
    sh_light_ = makeU<Shader>();
    sh_light_->create("res/shaders/shadows/shadow_map.vert", "res/shaders/shadows/shadow_map.frag");
    //
    sh_depth_ = makeU<Shader>();
    sh_depth_->create("res/shaders/shadows/simple_depth.vert", "res/shaders/shadows/simple_depth.frag");
    //
    sh_fbo_ = makeU<Shader>();
    sh_fbo_->create("res/shaders/framebuffer.vert", "res/shaders/shadows/depth_fbo.frag");

    // FLOOR TEXTURE CREATING
    Image image;
    image.load("res/images/Prototype_Grid_Gray_03-512x512.png");
    floor_texture_ = makeU<Texture>();
    TextureParams t_params_;
    t_params_.min_filter = GL_LINEAR_MIPMAP_LINEAR;
    floor_texture_->create(image, t_params_);
    floor_texture_->activeUnit(1);
    floor_texture_->bind();

    // FLOOR CREATING
    floor_ = makeU<Sprite>();
    floor_->setTextureSize(floor_texture_->getWidth(), floor_texture_->getHeight());
    floor_->transform.setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    floor_->transform.rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    floor_->transform.setScale(glm::vec3(floor_size_));
    floor_->setRegion(0.0f, 0.0f, floor_texture_->getWidth() * floor_size_, floor_texture_->getHeight() * floor_size_);
    floor_->generate();

    // BOX CREATING
    box_ = makeU<Voxel>(glm::vec3(0.0f, 1.0f, 0.0f));


    // SHADER SETTING UP
    sh_light_->uniform1i("f_texture", floor_texture_->getUnitId());

    // DEPTH MAP TEXTURE
    


}
void ShadowMap_sc::update(float delta) {
    
    if (input_->justPressed(GLFW_KEY_TAB))
    {
        window_->toggleCursor();    
    }
    if (input_->isCursorLocked())
    {
        camera_->process3DMouseRotation(input_->getDeltaX(), input_->getDeltaY());

        if (input_->pressed(GLFW_KEY_W))
        {
            camera_->translate(camera_->getFront() * camera_speed_ * delta);
        }
        if (input_->pressed(GLFW_KEY_S))
        {
            camera_->translate(-camera_->getFront() * camera_speed_ * delta);
        }
        if (input_->pressed(GLFW_KEY_A))
        {
            camera_->translate(-camera_->getRight() * camera_speed_ * delta);
        }
        if (input_->pressed(GLFW_KEY_D))
        {
            camera_->translate(camera_->getRight() * camera_speed_ * delta);
        }
        
    }
    
}


void ShadowMap_sc::draw() {
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    sh_light_->setMatrices(camera_->getProjectionMatrix(), camera_->getViewMatrix());
    sh_light_->uniformMatrix("model", floor_->transform.getModel());
    floor_->draw();
    //
    sh_light_->uniformMatrix("model", box_->transform.getModel());
    box_->draw();
}