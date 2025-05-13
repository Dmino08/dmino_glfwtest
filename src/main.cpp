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
//   position            normals              coord    
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f
};

    int attributes[] = {3,3,2};
    int size = sizeof(vertices);
    Mesh* cube = new Mesh(vertices, size, attributes, 3);
    Mesh* light = new Mesh(vertices, size, attributes, 3);
    

    InputManager& input = window.getInput();

// COLORS
    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    auto light_shader = Shader::create("res/shaders/light.vert", "res/shaders/light.frag");
    if (light_shader == nullptr)
    {
        logger.log(Logger::ERROR, "Light shader is not created");
        return -1;
    }    

    light_shader->use();
    light_shader->uniform3f("color", light_color);

    glm::mat4 model_light = glm::mat4(1.0f);
    glm::vec3 light_pos = glm::vec3(2.0f, 2.0f, 1.0f);
    model_light = glm::translate(glm::mat4(1.0f), light_pos);
    model_light = glm::scale(model_light, glm::vec3(0.3f, 0.3f, 0.3f));


    light_shader->uniformMatrix("model", model_light);
    light_shader->uniformMatrix("view", camera.getViewMatrix());
    light_shader->uniformMatrix("projection", camera.getProjectionMatrix());    



// TOY SHADER
    auto main_shader = Shader::create("res/shaders/light_test.vert", "res/shaders/point_light.frag");
    if (main_shader == nullptr)
    {
        logger.log(Logger::ERROR, "Toy shader is not created");
        return -1;
    }
    main_shader->use();
       


// Light setup
    main_shader->uniform3f("view_pos", camera.getPos());
    main_shader->uniform3f("light.position", camera.getPos() + camera.getFront());
    main_shader->uniform3f("light.ambient", 0.3f, 0.3f, 0.3f);
    main_shader->uniform3f("light.diffuse", 0.5f, 0.5f, 0.5f);
    main_shader->uniform3f("light.specular", 1.0f, 1.0f, 1.0f);
    main_shader->uniform1f("light.constant", 1.0f);
    main_shader->uniform1f("light.linear", 0.09f);
    main_shader->uniform1f("light.quadratic", 0.032f);

// Material setup
    main_shader->uniform1i("material.diffuse", 0);
    main_shader->uniform1i("material.specular", 1);
    main_shader->uniform1f("material.shininess", 32);

    

// TOY MODEL
    glm::mat4 main_model = glm::mat4(1.0f);


    main_shader->uniformMatrix("model", main_model);
    main_shader->uniformMatrix("view", camera.getViewMatrix());
    main_shader->uniformMatrix("projection", camera.getProjectionMatrix());



// GENERATING TEXTURE
    auto texture0 = Texture::create("res/images/container2.png");    
    auto texture1 = Texture::create("res/images/container2_specular.png");

// Input setting up
    float delta_time = 0.16f;
    float camera_speed = 5.f;

    glfwSwapInterval(1);

    glm::vec3 cube_positions[10];
    cube_positions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    cube_positions[1] = glm::vec3(2.0f, 2.0f, 0.0f);
    cube_positions[2] = glm::vec3(0.0f, 5.0f, 5.0f);
    cube_positions[3] = glm::vec3(6.0f, 0.0f, 6.0f);
    cube_positions[4] = glm::vec3(5.0f, 5.0f, 0.0f);
    cube_positions[5] = glm::vec3(7.0f, 7.0f, 0.0f);
    cube_positions[6] = glm::vec3(0.0f, 2.0f, 0.0f);
    cube_positions[7] = glm::vec3(5.0f, 5.0f, 5.0f);
    cube_positions[8] = glm::vec3(4.0f, 4.0f, 0.0f);
    cube_positions[9] = glm::vec3(0.0f, 10.0f, 0.0f);

// Loop
    while (!window.shouldClose())
    {
        window.pollEvents();

        auto start = std::chrono::high_resolution_clock::now();
        
        glClearColor(0.42, 0.42, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        if (input.justPressed(GLFW_KEY_TAB)) {
            input.toggleCursor();
        }
        if (input.isCursorLocked()) {

            camera.process3DMouseRotation(input.getDeltaX(), input.getDeltaY());

            if (input.pressed(GLFW_KEY_W)) {
                camera.translate(camera_speed * camera.getFront() * delta_time);
            }
            if (input.pressed(GLFW_KEY_S)) {
                camera.translate(-camera_speed * camera.getFront() * delta_time);
            }
            if (input.pressed(GLFW_KEY_A)) {
                camera.translate(-camera_speed * camera.getRight() * delta_time);
            }
            if (input.pressed(GLFW_KEY_D)) {
                camera.translate(camera_speed * camera.getRight() * delta_time);
            }   

            camera.toZoom(input.getScrollDeltaY() * 0.05f * delta_time);

        }     

        light_pos.x = sin(glfwGetTime()) * 3.5f;
        light_pos.y = cos(glfwGetTime()) * 3.5f;
        model_light = glm::translate(glm::mat4(1.0f), light_pos);
        model_light = glm::scale(model_light, glm::vec3(0.3f, 0.3f, 0.3f));
       
        


        main_shader->use();
        main_shader->uniform3f("view_pos", camera.getPos());
        main_shader->uniform3f("light.position", camera.getPos() + camera.getFront());
    //    main_shader->uniformMatrix("model", main_model);
        main_shader->uniformMatrix("view", camera.getViewMatrix());
        main_shader->uniformMatrix("projection", camera.getProjectionMatrix());


        glActiveTexture(GL_TEXTURE0);
        texture0->bind();
        glActiveTexture(GL_TEXTURE1);
        texture1->bind();
        
        for (size_t i = 0; i < 10; i++)
        {
            main_model = glm::mat4(1.0f);
            main_model = glm::translate(main_model, cube_positions[i]);

            float angle = 20 * i;

            main_model = glm::rotate(main_model, glm::radians(angle),
                                     glm::vec3(1.0f, 0.3f, 0.5f));
            main_shader->uniformMatrix("model", main_model);

            cube->draw();
        }
        

        

        texture0->unbind();
        texture1->unbind();


        // light_shader->use();
        // light_shader->uniformMatrix("model", model_light);
        // light_shader->uniformMatrix("view", camera.getViewMatrix());
        // light_shader->uniformMatrix("projection", camera.getProjectionMatrix()); 

        // light->draw();

        window.swapBuffers();

        auto end = std::chrono::high_resolution_clock::now();

        auto delta = std::chrono::duration<double>(end - start);
        delta_time = delta.count();

        window.setTitle(std::to_string(1.0f / delta_time));        

        
    }



    glfwTerminate();
    return 0;
}