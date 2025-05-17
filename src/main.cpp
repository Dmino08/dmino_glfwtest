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
                         glm::vec3* point_positions,
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
    for (size_t i = 0; i < 4; i++)
    {
        std::string base = "point_lights[" + std::to_string(i) + "].";

        shd.uniform3f(base + "base.ambient", ambient);
        shd.uniform3f(base + "base.diffuse", diffuse);
        shd.uniform3f(base + "base.specular", specular);

        shd.uniform3f(base + "position", point_positions[i]);

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

void setUpMatrices(Shader& shd, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) {
    shd.use();
    shd.uniformMatrix("model", model);
    shd.uniformMatrix("view", view);
    shd.uniformMatrix("projection", projection);    
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



// Vertex data
    util::Buffer<Vertex> cubeVertices = {
        // ЗАДНЯЯ грань (нормаль: -Z)
        {{-0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {0.0f, 1.0f}},

        // ПЕРЕДНЯЯ грань (нормаль: +Z)
        {{-0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {0.0f, 1.0f}},

        // ЛЕВАЯ грань (нормаль: -X)
        {{-0.5f, -0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},

        // ПРАВАЯ грань (нормаль: +X)
        {{ 0.5f, -0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},

        // НИЖНЯЯ грань (нормаль: -Y)
        {{-0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},

        // ВЕРХНЯЯ грань (нормаль: +Y)
        {{-0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
    };
// Indices data
    util::Buffer<uint> cubeIndices = {
        0,  1,  2,  2,  3,  0,      // задняя
        4,  5,  6,  6,  7,  4,      // передняя
        8,  9, 10, 10, 11,  8,      // левая
        12, 13, 14, 14, 15, 12,     // правая
        16, 17, 18, 18, 19, 16,     // нижняя
        20, 21, 22, 22, 23, 20      // верхняя
    };

    MeshData meshData{std::move(cubeVertices), std::move(cubeIndices)};

//  Our crate
    Mesh* crate = new Mesh(meshData);

    // Crate positions
    glm::vec3 crate_positions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    // Crate model matrix
    glm::mat4 crate_model = glm::mat4(1.0f);


// Our light
    Mesh* light = new Mesh(meshData);

    // Point light positions
    glm::vec3 point_light_positions[] = {
        glm::vec3( 0.7f, 0.2f, 2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3( 0.0f, 0.0f, -3.0f)
    };

    // Crate model matrix
    glm::mat4 light_model = glm::mat4(1.0f);
    

// Multiple shader
    auto multiple_shader = Shader::create("res/shaders/light_test.vert","res/shaders/multiple_lights.frag");
    if (multiple_shader == nullptr) {
        return -1;
    }
    setUpMatrices(*multiple_shader, camera.getProjectionMatrix(), camera.getViewMatrix(), crate_model);
    // Material setting
    multiple_shader->uniform1i("material.diffuse", 0);
    multiple_shader->uniform1i("material.specular", 1);
    multiple_shader->uniform1f("material.shininess", 32.0f);

// Light shader
    auto light_shader = Shader::create("res/shaders/light.vert","res/shaders/light.frag");
    if (light_shader == nullptr) {
        return -1;
    }
    setUpMatrices(*light_shader, camera.getProjectionMatrix(), camera.getViewMatrix(), light_model);
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

        
        setUpMatrices(*multiple_shader, camera.getProjectionMatrix(), camera.getViewMatrix(), crate_model);


        setUpMultipleShader(*multiple_shader,
                            camera.getPos(),
                            glm::vec3(0.05f),
                            glm::vec3(0.8f),
                            glm::vec3(1.0f),
                            camera.getFront(),
                            camera.getPos(),
                            point_light_positions,
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

        

        for (size_t i = 0; i < 10; i++)
        {
            crate_model = glm::mat4(1.0f);
            crate_model = glm::translate(crate_model, crate_positions[i]);

            float angle = 20 * i;

            crate_model = glm::rotate(crate_model, glm::radians(angle),
                                     glm::vec3(1.0f, 0.3f, 0.5f));
            multiple_shader->uniformMatrix("model", crate_model);

            crate->draw();
        }
    
        texture0.unbind();
        texture1.unbind();


        setUpMatrices(*light_shader, camera.getProjectionMatrix(), camera.getViewMatrix(), light_model);

        for (size_t i = 0; i < 4; i++)
        {
            light_model = glm::mat4(1.0f);
            light_model = glm::translate(light_model, point_light_positions[i]);

            float angle = 20 * i;

            light_model = glm::rotate(light_model, glm::radians(angle),
                                     glm::vec3(1.0f, 0.3f, 0.5f));
            light_model = glm::scale(light_model, glm::vec3(0.2f));

            light_shader->uniformMatrix("model", light_model);

            light->draw();
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