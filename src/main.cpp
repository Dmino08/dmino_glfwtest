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


void setUpMatrices(Shader& shd, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) {
    shd.uniformMatrix("model", model);
    shd.uniformMatrix("view", view);
    shd.uniformMatrix("projection", projection);    
}

void setUpSpotLight(Shader& shd, 
                    glm::vec3 position, 
                    glm::vec3 direction, 
                    float cut_off, 
                    glm::vec3 ambient, 
                    glm::vec3 diffuse, 
                    glm::vec3 specular,
                    float constant,
                    float linear,
                    float quadratic
                   ) 
{
    shd.uniform3f("light.position", position);
    shd.uniform3f("light.direction", direction);
    shd.uniform1f("light.cut_off", cut_off);
    shd.uniform3f("light.ambient", ambient);
    shd.uniform3f("light.diffuse", diffuse);
    shd.uniform3f("light.specular", specular);

    shd.uniform1f("light.constant", constant); 
    shd.uniform1f("light.linear", linear);
    shd.uniform1f("light.quadratic", quadratic);   
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



// MAIN SHADER
    auto main_shader = Shader::create("res/shaders/light_test.vert", "res/shaders/spot_light.frag");
    if (main_shader == nullptr)
    {
        logger.log(Logger::ERROR, "Toy shader is not created");
        return -1;
    }
    main_shader->use();
       
    main_shader->uniform1i("material.diffuse", 0);
    main_shader->uniform1i("material.specular", 1);

// Light setup
    main_shader->uniform3f("view_pos", camera.getPos());
    setUpSpotLight(*main_shader, 
                    camera.getPos(), 
                    camera.getFront(), 
                    glm::cos(glm::radians(12.5f)), 
                    glm::vec3(0.28f),
                    glm::vec3(0.8f),
                    glm::vec3(1.0f),
                    1.0f,
                    0.09f,
                    0.032f
                  );

    

// TOY MODEL
    glm::mat4 main_model = glm::mat4(1.0f);
    setUpMatrices(*main_shader, camera.getProjectionMatrix(), camera.getViewMatrix(), main_model);

    

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

        
        

        main_shader->use();
        main_shader->uniform3f("view_pos", camera.getPos());
        main_shader->uniform3f("light.position", camera.getPos());
        main_shader->uniform3f("light.direction", camera.getFront());
        setUpMatrices(*main_shader, camera.getProjectionMatrix(), camera.getViewMatrix(), main_model);


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


        window.swapBuffers();

        auto end = std::chrono::high_resolution_clock::now();

        auto delta = std::chrono::duration<double>(end - start);
        delta_time = delta.count();

        window.setTitle(std::to_string(1.0f / delta_time));        

        
    }



    glfwTerminate();
    return 0;
}