#include <iostream>

#include "config.h"
#include "engine/Engine.hpp"
#include "window/Window.hpp"

#include "scenes/ShadowMap_sc.hpp"
#include "scenes/PointShadows_sc.hpp"
#include "scenes/VoidScene_sc.hpp"

#include "core/Logger.hpp"
#include "core/MemoryTracker.hpp"

#include <iostream>
#include <string>
#include <functional>


void setFoo(std::string f_a, std::string s_a) {
    std::cout << f_a << s_a << '\n';
}
template <typename... Args>
void testFoo(std::function<void> func) {
    func(Args);
}

void windowLoop() 
{
    Window window = Window(1280, 720, "Window");

    core::Time time;

    ShadowMap_sc scene;
    Engine engine;
    scene.init(engine, window);


    while (!window.shouldClose())
    {
        time.update();

        glfwPollEvents();

        float delta = time.getDeltaTime();

        scene.preUpdate(delta);
        scene.input(window.getInput(), delta);
        scene.update(delta);
        scene.draw();
        scene.afterUpdate(delta);


        window.swapBuffers();
        window.eventsUpdate();

    }
    scene.onClose();


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
        engine.addScene<ShadowMap_sc>("1");
        engine.addScene<PointShadows_sc>("2");

        auto wind1 = std::make_unique<Window>(width, height, "Test");

        engine.addWindow("1", std::move(wind1));

        engine.attachSceneToWindow("1", "1");

        engine.run(); 


        print_Alloc_Memory_Kilobyte();
        print_Dealloc_Memory_Kilobyte();
        print_Usage_Memory_Kilobyte();  
    }
    Window::terminateGLFW();


    print_Alloc_Memory_Megabyte();
    print_Dealloc_Memory_Megabyte();
    print_Usage_Memory_Byte(); 

    return 0;
}