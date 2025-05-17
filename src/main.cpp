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
#include "graphics/core/Texture.hpp"
#include "graphics/core/Mesh.hpp"

#include "utils/Transform.hpp"

#include "test/Voxel.hpp"


#include "stb_image.h"



int width = 1280;
int height = 720;
Camera camera = Camera(CameraParams());




void updateTitle(Window& window, float timer, float& timerElapsed, float deltaTime) {
    timerElapsed += deltaTime;
    if (timer <= timerElapsed) {
        window.setTitle("FPS:" + std::to_string(int(1.0f/deltaTime)));
        timerElapsed = 0.0f;
    }

}

void setUpMultipleShader(Shader& shd, 
                         glm::vec3 view_pos,
                         glm::vec3 ambient,
                         glm::vec3 diffuse,
                         glm::vec3 specular,
                         glm::vec3 direciton,
                         glm::vec3 camera_position,
                         util::Buffer<Voxel> voxels,
                         float constant, 
                         float linear,
                         float quadratic,
                         float cut_off,
                         float outer_cut_off
                        ) 
{
    shd.uniform3f("view_pos", view_pos);
    // direction_light
    shd.uniform3f("direction_light.base.ambient", glm::vec3(0.05f));
    shd.uniform3f("direction_light.base.diffuse", glm::vec3(0.4f));
    shd.uniform3f("direction_light.base.specular", glm::vec3(0.5f));

    shd.uniform3f("direction_light.direction", glm::vec3(0.0f,-1.0f, 0.0f));
    // point_lights
    for (size_t i = 0; i < voxels.size(); i++)
    {
        std::string base = "point_lights[" + std::to_string(i) + "].";

        shd.uniform3f(base + "base.ambient", ambient);
        shd.uniform3f(base + "base.diffuse", diffuse);
        shd.uniform3f(base + "base.specular", specular);

        shd.uniform3f(base + "position", voxels[i].transform.getPosition());

        shd.uniform1f(base + "attenuation.constant", constant);
        shd.uniform1f(base + "attenuation.linear", linear);
        shd.uniform1f(base + "attenuation.quadratic", quadratic);
    }
    // spot_light
    shd.uniform3f("spot_light.base.ambient", glm::vec3(0.0f));
    shd.uniform3f("spot_light.base.diffuse", glm::vec3(1.0f));
    shd.uniform3f("spot_light.base.specular", glm::vec3(1.0f));

    shd.uniform1f("spot_light.cut_off", cut_off);
    shd.uniform1f("spot_light.outer_cut_off", outer_cut_off);    

    shd.uniform1f("spot_light.attenuation.constant", constant);
    shd.uniform1f("spot_light.attenuation.linear", linear);
    shd.uniform1f("spot_light.attenuation.quadratic", quadratic);

    shd.uniform3f("spot_light.direction", direciton);    
    shd.uniform3f("spot_light.position", camera_position); 
    
}

void setUpLightShader(Shader& shd, glm::vec3 color) {
    shd.uniform3f("color", color);
}


void frameBufferCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void countDelta(float& delta) {
    while (true)
    {
        std::cout << delta << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;

    if(!glfwInit()) {
        logger.log(Logger::ERROR, "GLFW is not initialized");
        return -1;
    }


    Window window = Window(width, height, "Window");
    if(!window.isValid()) {
        return -1;
    }
    logger.log(Logger::INFO, "Window was created");
    
    stbi_set_flip_vertically_on_load(true);
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    window.setFramebufferSizeCallback(frameBufferCallback);
//    window.setScrollCallback(scrollCallback);

// GENERATING TEXTURE
    Texture texture0 = Texture::create("res/images", "container2.png");    
    Texture texture1 = Texture::create("res/images", "container2_specular.png");


//  Our blocks
    util::Buffer<Voxel> crates = {
        {glm::vec3( 0.0f,  0.0f,  0.0f)},
        {glm::vec3( 2.0f,  5.0f, -15.0f)},
        {glm::vec3(-1.5f, -2.2f, -2.5f)},
        {glm::vec3(-3.8f, -2.0f, -12.3f)},
        {glm::vec3( 2.4f, -0.4f, -3.5f)},
        {glm::vec3(-1.7f,  3.0f, -7.5f)},
        {glm::vec3( 1.3f, -2.0f, -2.5f)},
        {glm::vec3( 1.5f,  2.0f, -2.5f)},
        {glm::vec3( 1.5f,  0.2f, -1.5f)},
        {glm::vec3(-1.3f,  1.0f, -1.5f)}
    };


// Our light
    util::Buffer<Voxel> lights = {
        {glm::vec3( 0.7f, 0.2f, 2.0f)},
        {glm::vec3( 2.3f, -3.3f, -4.0f)},
        {glm::vec3(-4.0f, 2.0f, -12.0f)},
        {glm::vec3( 0.0f, 0.0f, -3.0f)}
    };

    for (auto &i : lights)
    {
        i.transform.applyScale(0.2f, 0.2f, 0.2f);
    }
    
    

// Multiple shader
    auto multiple_shader = Shader::create("res/shaders/light_test.vert","res/shaders/multiple_lights.frag");
    if (multiple_shader == nullptr) {
        return -1;
    }
    multiple_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());
    // Material setting
    multiple_shader->uniform1i("material.diffuse", 0);
    multiple_shader->uniform1i("material.specular", 1);
    multiple_shader->uniform1f("material.shininess", 32.0f);

// Light shader
    auto light_shader = Shader::create("res/shaders/light.vert","res/shaders/light.frag");
    if (light_shader == nullptr) {
        return -1;
    }
    light_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());
    setUpLightShader(*light_shader, glm::vec3(1.0f));

//    Model model = Model("res/models/survival_guitar_backpack/scene.gltf");


// Input setting up
    InputManager& input = window.getInput();


// Some loop variables
   glfwSwapInterval(1);

    float timer = 1.0f;
    float timerElapsed = 0.0f;

    float camera_speed = 5.f;

    float deltaTime = 0.016f;
    int FPS = 60;

    auto targetDelta = std::chrono::microseconds(1000000 / FPS);

    std::thread delta_thread(countDelta, std::ref(deltaTime));
    delta_thread.detach();

// Loop 
    while (!window.shouldClose())
    { 
        auto frame_start = std::chrono::high_resolution_clock::now();
        updateTitle(window, timer, timerElapsed, deltaTime);

        window.pollEvents();

        glClearColor(0.42, 0.42, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        if (input.justPressed(GLFW_KEY_TAB)) {
            input.toggleCursor();
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

            camera.toZoom(input.getScrollDeltaY() * 0.05f * deltaTime);

        }     

        
        multiple_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());


        setUpMultipleShader(*multiple_shader,
                            camera.getPos(),
                            glm::vec3(0.05f),
                            glm::vec3(0.8f),
                            glm::vec3(1.0f),
                            camera.getFront(),
                            camera.getPos(),
                            lights,
                            1.0f,
                            0.09f,
                            0.032f,
                            glm::cos(glm::radians(7.5f)),
                            glm::cos(glm::radians(9.5f))
                        );

        glActiveTexture(GL_TEXTURE0);
        texture0.bind();
        glActiveTexture(GL_TEXTURE1);
        texture1.bind();

        
        

        for (size_t i = 0; i < crates.size(); i++)
        {
            float angle = 20 * i;

            crates[i].transform.setRotation(glm::vec3(1.0f * angle, 0.3f * angle, 0.5f * angle));

            multiple_shader->uniformMatrix("model", crates[i].transform.getModel());

            crates[i].draw();
        }
    
        texture0.unbind();
        texture1.unbind();


        light_shader->setMatrices(camera.getProjectionMatrix(), camera.getViewMatrix());


        for (size_t i = 0; i < lights.size(); i++)
        {
            light_shader->uniformMatrix("model", lights[i].transform.getModel());

            lights[i].draw();
        }



        window.swapBuffers();  
        


        auto frame_end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);

        // It doesn't work as it should
        // if (elapsed < targetDelta)
        // {
        //     std::this_thread::sleep_for(targetDelta - elapsed);
            
        //     frame_end = std::chrono::high_resolution_clock::now();
        //     elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);
        // }
        deltaTime = elapsed.count() / 1'000'000.0;
        
    }



    glfwTerminate();
    return 0;
}