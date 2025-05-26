#include "scenes/MainScene.hpp"

#include "window/Window.hpp"
#include "window/InputManager.hpp"


#include "graphics/core/Image.hpp"
#include "graphics/core/Texture.hpp"
#include "graphics/glsl/GLSLStructures.hpp"

#include <iostream>

void MainScene::init(Window& wind) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    std::cout << "Main is created\n";
    window = &wind;
    input = &wind.getInput();
    glfwSwapInterval(0);

    // CAMERA GENERATION
    CameraParams c_params;
    c_params.z_far = 15000.0f;
    camera = makeU<Camera>(wind, c_params);

    // SHADER GENERATION
    mult_shader = Shader::create("res/shaders/light_test.vert","res/shaders/multiple_lights.frag");


    // IMAGE GENERATION
    image0 = makeS<Image>();
    image0->load("res/images/Prototype_Grid_Gray_03-512x512.png");

    // TEXTURE GENERATION
    TextureParams tParams;
    tParams.min_filter = GL_LINEAR_MIPMAP_LINEAR;
    tParams.internal_format = GL_RGBA;
    texture0.create(*image0, tParams);

    // FLOOR GENERATION
    const float floor_size = 10000.0f;
    floor = makeU<Sprite>();
    floor->setTexture(&texture0);
    floor->transform.setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    floor->transform.setScale(glm::vec3(floor_size));
    floor->transform.rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    floor->setRegion(0,0, 512 * int(floor_size), 512 * int(floor_size));
    floor->generate();

    // LIGHT GENERATION  DIRECTIONAL
    glsl::DirectionalLight direction_light;
    direction_light.base.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    direction_light.base.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    direction_light.base.specular = glm::vec3(0.5f, 0.5f, 0.5f);

    direction_light.direction = glm::vec3(0.0f, -1.0f, 0.0f);

    // LIGHT GENERATION  POINT
    glsl::PointLight point_light;
    point_light.base.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    point_light.base.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    point_light.base.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    point_light.attenuation.constant = 1.0f;
    point_light.attenuation.linear = 0.09f;
    point_light.attenuation.quadratic = 0.032f;

    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);

    // LIGHT GENERATION  SPOT
    glsl::SpotLight spot_light;
    spot_light.base.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    spot_light.base.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    spot_light.base.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    spot_light.cut_off = 7.5f;
    spot_light.outer_cut_off = 9.5f;

    spot_light.attenuation.constant = 1.0f;
    spot_light.attenuation.linear = 0.009f;
    spot_light.attenuation.quadratic = 0.0032f;

    spot_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    spot_light.direction = glm::vec3(0.0f, -1.0f, 0.0f);

    // SHADER SET UP
    mult_shader->setMatrices(camera->getProjectionMatrix(), camera->getViewMatrix());
    glsl::setMaterial(*mult_shader, "material", glsl::Material{0, 1, 32.0f});
    // LIGHT SET UP
    glsl::setDirectionalLight(*mult_shader, "direction_light", direction_light);
    glsl::setSpotLight(*mult_shader, "spot_light", spot_light);

    mult_shader->uniform1i("on_flash_light", true);

    // TEXTURE BINDING
    glActiveTexture(GL_TEXTURE0);
    texture0.bind();

}

float fpsTimer = 0.0f;

void MainScene::update(float delta) {

    fpsTimer += delta;
    if (fpsTimer >= 1.0f) {
        fpsTimer = 0.0f;
        std::string title = "FPS: " + std::to_string(int(1.0f / delta));
        window->setTitle(title);
    }

    if (window->isResized()) {
            camera->updateProjection();
    }
    if (input->justPressed(GLFW_KEY_TAB)) {
        window->toggleCursor();
    }
    if (input->isCursorLocked()) {

        camera->process3DMouseRotation(input->getDeltaX(), input->getDeltaY());

        if (input->pressed(GLFW_KEY_W)) {
            camera->translate(camera_speed * camera->getFront() * delta);
        }
        if (input->pressed(GLFW_KEY_S)) {
            camera->translate(-camera_speed * camera->getFront() * delta);
        }
        if (input->pressed(GLFW_KEY_A)) {
            camera->translate(-camera_speed * camera->getRight() * delta);
        }
        if (input->pressed(GLFW_KEY_D)) {
            camera->translate(camera_speed * camera->getRight() * delta);
        }   

        if (input->justPressed(GLFW_KEY_C))
        {
            onFlashLight = !onFlashLight;

            mult_shader->use();
            mult_shader->uniform1i("on_flash_light", onFlashLight);
        }

        if(input->justPressed(GLFW_KEY_LEFT_SHIFT)) {
            camera_speed *= 3;
        }
        if(input->justReleased(GLFW_KEY_LEFT_SHIFT)) {
            camera_speed /= 3;
        }

        if(input->justPressed(GLFW_KEY_F)) {
            vsync = !vsync;
            glfwSwapInterval(vsync ? 1 : 0);
        }  
    }   
    camera->toZoom(-input->getScrollDeltaY(), 1.0f, 20.0f);

    mult_shader->setMatrices(camera->getProjectionMatrix(), camera->getViewMatrix());

    mult_shader->uniform3f("spot_light.position", camera->getPos());
    mult_shader->uniform3f("spot_light.direction", camera->getFront());

    mult_shader->uniform1i("material.diffuse", 0);
    mult_shader->uniform1i("use_specular_map", false);    

    mult_shader->uniformMatrix("model", floor->transform.getModel());
    floor->draw();
}