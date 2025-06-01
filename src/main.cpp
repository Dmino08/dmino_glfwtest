#include <iostream>

#include "config.h"
#include "engine/Engine.hpp"

#include "graphics/core/VertexStructures.hpp"
#include "window/Window.hpp"
#include "graphics/render/FrameBuffer.hpp"
#include "graphics/core/CubeMap.hpp"

#include "scenes/MainScene.hpp"
#include "scenes/AOPGScene.hpp"

void windowLoop() {
    Window window = Window(1280, 720, "Window");

    core::Time time;
    
    Engine engine1;
    AOPGScene scene = AOPGScene(engine1);  
    scene.init(window); 

    int frames = 0;
    float elapsed = 0.0f;


    while (!window.shouldClose())
    {
        time.update();
        
        glfwPollEvents();

        scene.update(time.getDeltaTime());
        scene.draw();


        window.swapBuffers();
        
        window.eventsUpdate();

        frames++;
        elapsed  += time.getDeltaTime();
        if (elapsed >= 60.0f) {
            core::logger.log(core::Logger::INFO, "FBO Frames: " + std::to_string(frames));
            elapsed = 0.0f;
            // break;
        }
    }
}



int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;
    
    int width = 1280;
    int height = 720;

    Window::initGLFW();
    {
        Engine engine;
        auto wind1 = std::make_unique<Window>(width, height, "Window1");

            uint ubo_buffer;
            glGenBuffers(1, &ubo_buffer);
            glBindBuffer(GL_UNIFORM_BUFFER, ubo_buffer);
            glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_buffer, 0, 2 * sizeof(glm::mat4));
            engine.getAssets().addItem<int>(std::make_shared<int>(ubo_buffer), "Matrices");

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        
        engine.addScene<MainScene>("1");
        engine.addScene<AOPGScene>("2");

        engine.addWindow("1", std::move(wind1));

        engine.attachSceneToWindow("1", "1");

        glEnable(GL_PROGRAM_POINT_SIZE);
        engine.run();

        glDeleteBuffers(1, &ubo_buffer);
    }


    Window::terminateGLFW();
    return 0;
}