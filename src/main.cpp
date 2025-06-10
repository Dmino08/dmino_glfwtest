#include <iostream>

#include "config.h"
#include "engine/Engine.hpp"
#include "window/Window.hpp"

#include "scenes/ShadowMap_sc.hpp"

#include "core/Logger.hpp"



void windowLoop() 
{
    Window window = Window(1280, 720, "Window");

    core::Time time;


    while (!window.shouldClose())
    {
        time.update();

        glfwPollEvents();

        window.swapBuffers();

        window.eventsUpdate();

    }

    print_Alloc_Memory_Kilobyte();
    print_Dealloc_Memory_Kilobyte();
    print_Usage_Memory_Kilobyte();
}

int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;
    
    int width = 1152;
    int height = 648;

    Window::initGLFW();
    {
        Engine engine;
        auto wind1 = std::make_unique<Window>(width, height, "Test");

        engine.addWindow("1", std::move(wind1));
        engine.addScene<ShadowMap_sc>("1");

        engine.attachSceneToWindow("1", "1");

        engine.run();
    }
    // windowLoop();

    Window::terminateGLFW();

    return 0;
}