#include "engine/Engine.hpp"
 
#include "window/Window.hpp"
#include "engine/IScene.hpp"
#include "core/Logger.hpp"

#include <iostream>

Engine::~Engine() {}

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
        pair.second->init(*this, *pair.first);
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
    return assets_;
}

core::Time& Engine::getTime() {
    return time_;
}

void Engine::run() {
    int frames = 0;
    float elapsed = 0.0f;
    
    while (!should_end_)
    {
        time_.update();
        float delta = time_.getDeltaTime();

        glfwPollEvents(); 

        for (auto it = pairs_.begin(); it != pairs_.end(); )
        {
            auto& [name, pair] = *it;

            if (pairs_.size() > 1) {
                pair.first->makeContextCurrent();
            }     
            
            if (!pair.first->shouldClose()) {

                pair.second->preUpdate(delta);

                pair.second->update(delta);
                pair.second->input(pair.first->getInput(), delta);
                pair.second->draw();

                pair.second->afterUpdate(delta);

                pair.first->swapBuffers();
                
                pair.first->eventsUpdate();

                ++it;
            }
            else {
                pair.second->onClose();
                it = pairs_.erase(it);  

                if (!pairs_.empty()) {
                    pairs_.begin()->second.first->makeContextCurrent();
                }                      
            }
        }

        if (pairs_.empty()) {
            should_end_ = true;
        }
        frames++;
        elapsed  += time_.getDeltaTime();
        if (elapsed >= 60.0f) {
            core::logger.log(core::Logger::INFO, "Engine Frames: " + std::to_string(frames));
            elapsed = 0.0f;
            // break;
        }
    }
}