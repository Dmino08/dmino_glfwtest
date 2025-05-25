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

#include "assets/Assets.hpp"
#include "engine/Engine.hpp"
#include "engine/IScene.hpp"

#include <random>
#include <map>


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

class MainScene : public IScene {
    public:
        MainScene(Engine& engine) : IScene(engine) {}
};


int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;

    int width = 1280;
    int height = 720;
    
    Window::initGLFW();
    Image::flipLoad(true);

    Engine engine;
    std::cout << sizeof(Engine) << "\n";

    auto wind1 = std::make_unique<Window>(width, height, "Window1");
    auto wind2 = std::make_unique<Window>(width, height, "Window2");

    engine.addScene<MainScene>("scene0");

    engine.addWindow("wind1", std::move(wind1));
    engine.addWindow("wind2", std::move(wind2));

    auto window1 = engine.getWindow("wind1");
    if (window1) {
        Window& r_wind1 = window1->get();
        std::cout << "Hello wind1\n";
    }
    else {
        std::cout << "Window1 is not here\n";
    }
    



    Window window = Window(width, height, "Window");
    if(!window.isValid()) {
        return -1;
    }

// Input setting up
    InputManager& input = window.getInput();

// Some loop variables
    glfwSwapInterval(1);
    bool vsync = true;

    float timer = 1.0f;
    float timerElapsed = 0.0f;

    float camera_speed = 10.f;

    float deltaTime = 0.016f;
    int FPS = 120;

    using clock = std::chrono::steady_clock;

    auto targetDelta = std::chrono::microseconds(1'000'000 / FPS);
    std::cout << "TARGET: " << targetDelta.count() << std::endl;

    std::thread delta_thread(countDelta, std::ref(deltaTime));
    delta_thread.detach();

    bool onFlashLight = true;

//CREATING FRAMEBUFFER......................................................................................
    unsigned fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "FRAMEBUFFER GENERATION IS COMPLETE\n";
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


//SETTING OPENGL......................................................................................
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
// CHECK MY ASSETS STUFF
   

//......................................................................................SETTING OPENGL
// Loop 
    while (!window.shouldClose())
    { 
    //LOOP BEGINNING......................................................................................
        auto frameStart = clock::now();
        updateTitle(window, timer, timerElapsed, deltaTime);

        window.pollEvents();

        glClearColor(0.42, 0.42, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        

        window.swapBuffers();  
        auto frameEnd = clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
        deltaTime = float(elapsed.count()) / 1'000'000.0;    
    }
    glDeleteFramebuffers(1, &fbo);

    glfwTerminate();
    return 0;
}