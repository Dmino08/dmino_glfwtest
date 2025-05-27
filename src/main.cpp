#include <iostream>

#include "config.h"
#include "engine/Engine.hpp"
#include "scenes/MainScene.hpp"

int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;

    int width = 1280;
    int height = 720;

    Engine engine;

    auto wind1 = std::make_unique<Window>(width, height, "Window1");

    engine.addScene<MainScene>("main");

    engine.addWindow("window1", std::move(wind1));

    engine.attachSceneToWindow("main", "window1");

    engine.run();

    return 0;
}