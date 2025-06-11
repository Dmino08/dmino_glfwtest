#include "scenes/PointShadows_sc.hpp"

#include "graphics/core/Shader.hpp"
#include "graphics/core/Texture.hpp"


#include "graphics/glsl/GLSLHelper.hpp"
#include "graphics/core/VertexStructures.hpp"
#include "core/Logger.hpp"


PointShadows_sc::PointShadows_sc() : sides_()
{}

PointShadows_sc::~PointShadows_sc() = default;


void PointShadows_sc::init(Engine& engine, Window& window)
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
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    
    // GLFW SET UP
    glfwSwapInterval(0);

        // CAMERA SET UP
    CameraParams c_params;
    c_params.z_far = 15'000.0f;
    camera_ = makeU<Camera>(window, c_params);
    camera_->setTransform(glm::vec3(0.0f, 1.0f, 10.0f));

    // SHADER SET UP
    sh_main_ = makeU<Shader>();
    sh_main_->create("res/shaders/basic/main.vert", "res/shaders/basic/main.frag");

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
    glActiveTexture(GL_TEXTURE1);
    simple_texture_->create(image, t_params);
    simple_texture_->bind();
    //
    Sprite side;
    side.setRegion(0, 0, side_size_, side_size_);
    side.transform.setScale(glm::vec3(side_size_));
    side.generate();
    //
    sides_.fill(side);
    // FLOOR
    sides_[0].transform.setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    sides_[0].transform.setPosition(glm::vec3(0.0f, -side_size_ / 2, 0.0f));
    // CEILING
    sides_[1].transform.setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
    sides_[1].transform.setPosition(glm::vec3(0.0f, side_size_ / 2, 0.0f));
    // RIGHT
    sides_[2].transform.setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    sides_[2].transform.setPosition(glm::vec3(side_size_ / 2, 0.0f, 0.0f));
    // LEFT
    sides_[3].transform.setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    sides_[3].transform.setPosition(glm::vec3(-side_size_ / 2, 0.0f, 0.0f));
    // FAR
    sides_[4].transform.setRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    sides_[4].transform.setPosition(glm::vec3(0.0f, 0.0f, side_size_ / 2));
    // NEAR
    sides_[5].transform.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    sides_[5].transform.setPosition(glm::vec3(0.0f, 0.0f, -side_size_ / 2));

    // BOXES SET UP
    image.load("res/images/container2.png");
    box_texture_ = makeU<Texture>();
    box_texture_->activeUnit(2);
    box_texture_->create(image, t_params);
    box_texture_->bind();
    //
    boxes_[0] = Voxel(glm::vec3(5.0f, 2.0f, 0.0f));
    boxes_[1] = Voxel(glm::vec3(-5.0f, -2.0f, 2.0f));
    boxes_[2] = Voxel(glm::vec3(5.0f, 0.0f, -5.0f));
    boxes_[3] = Voxel(glm::vec3(2.0f, 0.0f, 5.0f));

    // FBO SET UP
    fbo_.create(POINT_DEPTH_PARAMS, SHADOW_K_1, SHADOW_K_1, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT);

    // LIGHT SET UP
    dpl_.shad_proj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);

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


}

void PointShadows_sc::update(float delta)
{

}

void PointShadows_sc::draw()
{   
    glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    

    sh_main_->use();
    sh_main_->uniformMat4(PROJ_VIEW, camera_->getProjView());

    sh_main_->uniform1i(TEXTURE, 1);
    for (auto& side : sides_)
    {
        sh_main_->uniformMat4(MODEL, side.transform.getModel());
        side.draw();        
    }
    
    sh_main_->uniform1i(TEXTURE, 2);
    for (auto& box : boxes_)
    {
        sh_main_->uniformMat4(MODEL, box.transform.getModel());
        box.draw();  
    }
    
}

void PointShadows_sc::onClose()
{
    #ifdef MEMORY_DEBUG
        print_Alloc_Memory_Kilobyte();
        print_Dealloc_Memory_Kilobyte();
        print_Usage_Memory_Kilobyte();
    #endif    
}
