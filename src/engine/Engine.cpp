#include "engine/Engine.hpp"
 
#include "window/Window.hpp"
#include "engine/IScene.hpp"


void Engine::addWindow(const std::string& name, std::unique_ptr<Window>&& window) {
    windows_.insert_or_assign(name, std::move(window));
}

void Engine::attachSceneToWindow(const std::string& scene, const std::string& window) {
    pairs.insert_or_assign(window, scene);
}


std::optional<std::reference_wrapper<Window>> Engine::getWindow(const std::string& name) {
    auto it = windows_.find(name);

    if (it != windows_.end()){
        return std::ref(*it->second.get());
    }
    else {
        return std::nullopt;
    }
}

void Engine::run() {
    while (!shouldEnd)
    {
        
    }
}