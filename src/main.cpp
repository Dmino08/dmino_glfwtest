#include <iostream>

#include "config.h"
#include "engine/Engine.hpp"
#include "scenes/MainScene.hpp"
#include "graphics/core/VertexStructures.hpp"
#include "window/Window.hpp"
#include "graphics/render/FrameBuffer.hpp"
#include "graphics/core/CubeMap.hpp"

#include "test/Voxel.hpp"

void windowLoop() {
    Window window = Window(1280, 720, "Window");

    core::Time time;
    
    Engine engine1;
    u_ptr<MainScene> main = makeU<MainScene>(engine1);  
    main->init(window); 

    int frames = 0;
    float elapsed = 0.0f;
    while (!window.shouldClose())
    {
        time.update();
        
        glfwPollEvents();

        main->update(time.getDeltaTime());
        main->draw();


        window.swapBuffers();
        
        window.eventsUpdate();

        frames++;
        elapsed  += time.getDeltaTime();
        if (elapsed >= 60.0f) {
            core::logger.log(core::Logger::INFO, "FBO Frames: " + std::to_string(frames));
            break;
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
        
        engine.addScene<MainScene>("main");

        engine.addWindow("window1", std::move(wind1));

        engine.attachSceneToWindow("main", "window1");

        engine.run();
    }
    {
        windowLoop();
    }

    
    Window::terminateGLFW();
    return 0;
}