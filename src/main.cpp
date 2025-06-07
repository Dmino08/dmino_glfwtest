#include <iostream>

#include "config.h"
#include "engine/Engine.hpp"
#include "window/Window.hpp"

#include "scenes/ShadowMap_sc.hpp"


int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;
    
    int width = 1280;
    int height = 720;

    Window::initGLFW();
    {
        Engine engine;
        auto wind1 = std::make_unique<Window>(width, height, "Test");
        
        engine.addWindow("1", std::move(wind1));
        engine.addScene<ShadowMap_sc>("1");
        engine.attachSceneToWindow("1", "1");

        engine.run();
    }

    Window::terminateGLFW();
    return 0;
}