#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window/Window.hpp"
#include "window/Camera.hpp"
#include "window/InputManager.hpp"

#include "core/Logger.hpp"

#include "graphics/core/Shader.hpp"
#include "graphics/core/Image.hpp"
#include "graphics/core/Texture.hpp"
#include "graphics/core/Mesh.hpp"
#include "graphics/core/Sprite.hpp"

#include "graphics/glsl/GLSLStructures.hpp"

#include "utils/Transform.hpp"

#include "test/Voxel.hpp"

#include "stb_image.h"

void updateTitle(Window& window, float timer, float& timerElapsed, float deltaTime) {
    timerElapsed += deltaTime;
    if (timer <= timerElapsed) {
        window.setTitle("FPS:" + std::to_string(int(1.0f/deltaTime)));
        timerElapsed = 0.0f;
    }

}


void setUpLightShader(Shader& shd, glm::vec3 color) {
    shd.uniform3f("color", color);
}



void countDelta(float& delta) {
    while (true)
    {
        std::cout << delta << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
}

int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;

    if(!glfwInit()) {
        logger.log(Logger::ERROR, "GLFW is not initialized");
        return -1;
    }

    int width = 1920;
    int height = 1080;
    

    Window window = Window(width, height, "Window");
    if(!window.isValid()) {
        return -1;
    }
    logger.log(Logger::INFO, "Window was created");
    
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Camera camera = Camera(window, CameraParams());

// GENERATING TEXTURE
    Image image0;
    image0.load("res/images/container2.png");
    Image image1;
    image1.load("res/images/container2_specular.png");
    Image image2;
    image2.load("res/images/Prototype_Grid_Gray_03-512x512.png");

    Texture texture0 = Texture::create(image0);    
    Texture texture1 = Texture::create(image1);
    Texture texture2 = Texture::create(image2);

//  Our crates
    std::vector<Voxel> crates = {
        {glm::vec3( 0.0f,  0.0f,  0.0f)},
        {glm::vec3( 2.0f,  0.0f, -15.0f)},
        {glm::vec3(-1.5f,  0.0f, -2.5f)},
        {glm::vec3(-3.8f,  0.0f, -12.3f)},
        {glm::vec3( 2.4f,  0.0f, -3.5f)},
        {glm::vec3(-1.7f,  0.0f, -7.5f)},
        {glm::vec3( 1.3f,  0.0f, -2.5f)},
        {glm::vec3( 1.5f,  0.0f, -2.5f)},
        {glm::vec3( 1.5f,  0.0f, -1.5f)},
        {glm::vec3(-1.3f,  0.0f, -1.5f)},

        {glm::vec3(-7.3f,  0.0f, -5.5f)},
        {glm::vec3(-4.3f,  0.0f, 4.5f)},
        {glm::vec3(5.3f,   0.0f, -1.5f)},
        {glm::vec3(-1.3f,  0.0f, 4.5f)},
        {glm::vec3(12.3f,  0.0f, -1.5f)},
        {glm::vec3(4.3f,   0.0f, -1.5f)}
    };

// Light positions
    std::vector<Voxel> lights = {
        {glm::vec3( 0.7f, 0.2f, 2.0f)},
        {glm::vec3( 2.3f, -3.3f, -4.0f)},
        {glm::vec3(-4.0f, 2.0f, -12.0f)},
        {glm::vec3( 0.0f, 0.0f, -3.0f)}
    };

    for (auto &i : lights)
    {
        i.transform.applyScale(0.2f, 0.2f, 0.2f);
    }

// DirectionalLight
    glsl::DirectionalLight direction_light;
    direction_light.base.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    direction_light.base.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    direction_light.base.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    
    direction_light.direction = glm::vec3(0.0f, -1.0f, 0.0f);

// PointLight
    glsl::PointLight point_light;
    point_light.base.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    point_light.base.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    point_light.base.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    point_light.attenuation.constant = 1.0f;
    point_light.attenuation.linear = 0.09f;
    point_light.attenuation.quadratic = 0.032f;
    
    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);

// SpotLight
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


// Multiple shader
    auto multiple_shader = Shader::create("res/shaders/light_test.vert","res/shaders/multiple_lights.frag");
    if (multiple_shader == nullptr) {
        return -1;
    }
    multiple_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());
    // Material setting
    glsl::setMaterial(*multiple_shader, "material", glsl::Material{0, 1, 32.0f});

    // Light setting
    glsl::setDirectionalLight(*multiple_shader, "direction_light", direction_light);
    glsl::setSpotLight(*multiple_shader, "spot_light", spot_light);

    point_light.position = lights[0].transform.getPosition();
    glsl::setPointLight(*multiple_shader, "point_lights[0]", point_light);

    point_light.position = lights[1].transform.getPosition();
    glsl::setPointLight(*multiple_shader, "point_lights[1]", point_light);

    point_light.position = lights[2].transform.getPosition();
    glsl::setPointLight(*multiple_shader, "point_lights[2]", point_light);

    point_light.position = lights[3].transform.getPosition();
    glsl::setPointLight(*multiple_shader, "point_lights[3]", point_light);   

    multiple_shader->uniform1i("on_flash_light", true);
    multiple_shader->uniform1i("use_specular_map", true);

// Outline shader
    auto outline_shader = Shader::create("res/shaders/light_test.vert", "res/shaders/stencil_test.frag");
    outline_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());

// Light shader
    auto light_shader = Shader::create("res/shaders/ui.vert","res/shaders/light.frag");
    if (light_shader == nullptr) {
        return -1;
    }
    light_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());
    setUpLightShader(*light_shader, glm::vec3(1.0f));

// Creating sprite
    Sprite plane;
    plane.setTexture(&texture2);

    const float planeScale = 100.0f;
    plane.transform.setPosition(glm::vec3(0.0f, -0.5f, 0.0f));
    plane.transform.rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    plane.transform.setScale(glm::vec3(planeScale));

    plane.setRegion(0, 0, 512 * int(planeScale) / 4, 512 * int(planeScale) / 4);
    plane.generate();

// Input setting up
    InputManager& input = window.getInput();

// Some loop variables
    glfwSwapInterval(1);

    float timer = 1.0f;
    float timerElapsed = 0.0f;

    float camera_speed = 5.f;

    float deltaTime = 0.016f;
    int FPS = 120;

    using clock = std::chrono::steady_clock;

    auto targetDelta = std::chrono::microseconds(1'000'000 / FPS);
    std::cout << "TARGET: " << targetDelta.count() << std::endl;

    std::thread delta_thread(countDelta, std::ref(deltaTime));
    delta_thread.detach();

    bool onFlashLight = true;

    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);


// Loop 
    while (!window.shouldClose())
    { 
        auto frameStart = clock::now();
        updateTitle(window, timer, timerElapsed, deltaTime);

        window.pollEvents();
        
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                  
        glClearColor(0.42, 0.42, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        if (input.justPressed(GLFW_KEY_TAB)) {
            window.toggleCursor();
        }
        if (input.isCursorLocked()) {

            camera.process3DMouseRotation(input.getDeltaX(), input.getDeltaY());

            if (input.pressed(GLFW_KEY_W)) {
                camera.translate(camera_speed * camera.getFront() * deltaTime);
            }
            if (input.pressed(GLFW_KEY_S)) {
                camera.translate(-camera_speed * camera.getFront() * deltaTime);
            }
            if (input.pressed(GLFW_KEY_A)) {
                camera.translate(-camera_speed * camera.getRight() * deltaTime);
            }
            if (input.pressed(GLFW_KEY_D)) {
                camera.translate(camera_speed * camera.getRight() * deltaTime);
            }   

            if (input.justPressed(GLFW_KEY_C))
            {
                onFlashLight = !onFlashLight;

                multiple_shader->use();
                multiple_shader->uniform1i("on_flash_light", onFlashLight);
            }
            


            camera.toZoom(-input.getScrollDeltaY() * 0.05f * deltaTime);

        }     
        glStencilMask(0x00);
        light_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());
        for (size_t i = 0; i < lights.size(); i++)
        {
            light_shader->uniformMatrix("model", lights[i].transform.getModel());

            lights[i].draw();
        }

        multiple_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());
        multiple_shader->uniform3f("spot_light.position", camera.getPos());
        multiple_shader->uniform3f("spot_light.direction", camera.getFront());
        
        glActiveTexture(GL_TEXTURE0);
        texture2.bind();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        
        multiple_shader->uniformMatrix("model", plane.transform.getModel());
        
        plane.draw();        
        

        glActiveTexture(GL_TEXTURE0);
        texture0.bind();
        glActiveTexture(GL_TEXTURE1);
        texture1.bind();

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        for (size_t i = 0; i < crates.size(); i++)
        {
            multiple_shader->uniformMatrix("model", crates[i].transform.getModel());

            crates[i].draw();
        }
   
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        outline_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());
        for (size_t i = 0; i < crates.size(); i++)
        {
            auto outline_model = glm::scale(crates[i].transform.getModel(), glm::vec3(1.05f));
            outline_shader->uniformMatrix("model", outline_model);

            crates[i].draw();
        }
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);


        texture0.unbind();
        texture1.unbind();
        texture2.unbind();

         


        window.swapBuffers();  
        auto frameEnd = clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
        deltaTime = float(elapsed.count()) / 1'000'000.0;    
    }

    glfwTerminate();
    return 0;
}