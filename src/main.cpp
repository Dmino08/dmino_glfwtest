#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

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

#include "graphics/Shader.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Mesh.hpp"

#include "stb_image.h"


int width = 1280;
int height = 720;
Camera camera = Camera(CameraParams());


void drawXYCubes(Shader& shader, glm::mat4& model, int x, int y, int z) {
    const float offset = 2.0f;
    shader.use();
    for (size_t i = 1; i <= x; i++) { 
        for (size_t j = 1; j <= y; j++) {
            for (size_t q = 1; q <= z; q++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(i * offset, j * offset, q * offset));
                shader.uniformMatrix("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }
}

void frameBufferCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

// Creating a Cube
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

    Mesh* cube = new Mesh(vertices, sizeof(vertices));

    


    InputManager& input = window.getInput();


    auto shader = Shader::create("res/shaders/light_test.vert", "res/shaders/light_test.frag");
    if (shader == nullptr)
    {
        logger.log(Logger::ERROR, "Shader is not created");
        return -1;
    }
    shader->use();
    shader->uniform1i("texture1", 0);   
    shader->uniform1i("texture2", 1);

    shader->uniform3f("light_color", 0.0f, 1.0f, 0.0f);
    shader->uniform3f("toy_color", 1.0f, 1.0f, 1.0f);

// GLM HARD STUFF
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    shader->uniformMatrix("model", model);
    shader->uniformMatrix("view", camera.getViewMatrix());
    shader->uniformMatrix("projection", camera.getProjectionMatrix());


// GENERATING TEXTURE
    auto texture1 = Texture::create("res/images/Shreak.jpg");
    auto texture2 = Texture::create("res/images/Donkey.jpg");    
    

// Input setting up
    float delta_time = 0.16f;
    float camera_speed = 5.f;

    glfwSwapInterval(1);

// Loop
    while (!window.shouldClose())
    {
        window.pollEvents();


        auto start = std::chrono::high_resolution_clock::now();


        
        glClearColor(0.42, 0.42, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        shader->use();

        if (input.justPressed(GLFW_KEY_TAB)) {
            input.toggleCursor();
        }
        if (input.isCursorLocked()) {

            camera.process3DMouseRotation(input.getDeltaX(), input.getDeltaY());

            if (input.pressed(GLFW_KEY_W)) {
                camera.translate(camera_speed * delta_time * camera.getFront());
            }
            if (input.pressed(GLFW_KEY_S)) {
                camera.translate(-camera_speed * delta_time * camera.getFront() );
            }
            if (input.pressed(GLFW_KEY_A)) {
                camera.translate(-camera_speed * camera.getRight() * delta_time);
            }
            if (input.pressed(GLFW_KEY_D)) {
                camera.translate(camera_speed * camera.getRight() * delta_time);
            }   



            camera.toZoom(input.getScrollDeltaY() * 0.05f * delta_time);

        }     

        

        

        shader->uniformMatrix("view", camera.getViewMatrix());
        shader->uniformMatrix("projection", camera.getProjectionMatrix());

        glActiveTexture(GL_TEXTURE0);
        texture1->bind();
        glActiveTexture(GL_TEXTURE1);
        texture2->bind();
        
        cube->draw();

        texture1->unbind();
        texture2->unbind();

        window.swapBuffers();

        auto end = std::chrono::high_resolution_clock::now();

        auto delta = std::chrono::duration<double>(end - start);
        delta_time = delta.count();

        window.setTitle(std::to_string(1.0f / delta_time));        

        
    }



    glfwTerminate();
    return 0;
}