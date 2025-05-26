#include "engine/Engine.hpp"
 
#include "window/Window.hpp"
#include "engine/IScene.hpp"

Engine::Engine() {
    Window::initGLFW();  
}

Engine::~Engine() {
    glfwTerminate();
}

void Engine::addWindow(const std::string& name, std::unique_ptr<Window>&& window) {
    pairs_[name].first = std::move(window);
    pairs_[name].second = nullptr;
}

void Engine::attachSceneToWindow(const std::string& scene, const std::string& window) {
    if (factories_.find(scene) != factories_.end())
    {
        pairs_[window].first->makeContextCurrent();
        pairs_[window].second = factories_[scene]();
        pairs_[window].second->init(*pairs_[window].first);
    }
}

std::optional<std::reference_wrapper<Window>> Engine::getWindow(const std::string& name) {
    auto it = pairs_.find(name);

    if (it != pairs_.end()){
        return std::ref(*it->second.first.get());
    }
    else {
        return std::nullopt;
    }
}

Assets& Engine::getAssets() {
    return assets;
}

void Engine::run() {
    while (!shouldEnd)
    {
        time.update();
        float delta = time.getDeltaTime();
        
        glfwPollEvents();

        for (auto it = pairs_.begin(); it != pairs_.end(); )
        {
            auto& [name, pair] = *it;

            pair.first->makeContextCurrent();
            
            if (!pair.first->shouldClose()) {
                
                pair.second->preUpdate(delta);

                glClearColor(0.42, 0.42, 0.6, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                pair.second->update(delta);

                pair.first->swapBuffers();

                pair.second->afterUpdate(delta);
                
                pair.first->eventsUpdate();

                ++it;
            }
            else {
                it = pairs_.erase(it);
            }
        }

        if (pairs_.empty())
        {
            shouldEnd = true;
        }
        
    }
}