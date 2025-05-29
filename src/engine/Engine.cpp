#include "engine/Engine.hpp"
 
#include "window/Window.hpp"
#include "engine/IScene.hpp"
#include "core/Logger.hpp"

#include <iostream>

void Engine::addWindow(const std::string& name, std::unique_ptr<Window>&& window) {
    pairs_[name].first = std::move(window);
    pairs_[name].second = nullptr;
}

void Engine::attachSceneToWindow(const std::string& scene, const std::string& window) {
    if (factories_.find(scene) != factories_.end())
    {
        auto& pair = pairs_[window];

        pair.first->makeContextCurrent();
        pair.second = factories_[scene]();
        pair.second->init(*pair.first);
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
    int frames = 0;
    float timer = 0.0f;
    while (!shouldEnd)
    {
        time.update();
        float delta = time.getDeltaTime();

        glfwPollEvents(); 

        for (auto it = pairs_.begin(); it != pairs_.end(); )
        {
            auto& [name, pair] = *it;

            if (pairs_.size() > 1) {
                pair.first->makeContextCurrent();
            }     
            
            if (!pair.first->shouldClose()) {

                pair.second->preUpdate(delta);

                glClearColor(0.42, 0.42, 0.6, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                pair.second->update(delta);
                pair.second->draw();

                pair.second->afterUpdate(delta);

                pair.first->swapBuffers();
                
                pair.first->eventsUpdate();

                ++it;
            }
            else {
                it = pairs_.erase(it);  

                if (!pairs_.empty()) {
                    pairs_.begin()->second.first->makeContextCurrent();
                }                      
            }
        }

        if (pairs_.empty())
        {
            shouldEnd = true;
        }
        frames++;
    }
    core::logger.log(core::Logger::INFO, std::to_string(frames));
}