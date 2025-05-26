#include <iostream>

#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window/Window.hpp"
#include "window/InputManager.hpp"

#include "assets/Assets.hpp"
#include "engine/Engine.hpp"
#include "engine/IScene.hpp"

#include "scenes/MainScene.hpp"

#include <random>
#include <map>

int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;

    int width = 1280;
    int height = 720;

    Engine engine;


    auto wind1 = std::make_unique<Window>(width, height, "Window1");
    auto wind2 = std::make_unique<Window>(width, height, "Window2");
    auto wind3 = std::make_unique<Window>(width, height, "Window3");
    auto wind4 = std::make_unique<Window>(width, height, "Window4");

    engine.addScene<MainScene>("main");

    engine.addWindow("window1", std::move(wind1));
    engine.addWindow("window2", std::move(wind2));
    engine.addWindow("window3", std::move(wind3));
    engine.addWindow("window4", std::move(wind4));

    engine.attachSceneToWindow("main", "window1");
    engine.attachSceneToWindow("main", "window2");
    engine.attachSceneToWindow("main", "window3");
    engine.attachSceneToWindow("main", "window4");

    engine.run();

    return 0;
}