#pragma once

#include "assets/Assets.hpp"
#include <functional>


class Window;
class IScene;

class Engine {
    Assets assets;

    std::unordered_map<std::string, std::unique_ptr<Window>> windows_;
    std::vector<std::unique_ptr<IScene>> scenes_;

    std::unordered_map<std::string, std::function<std::unique_ptr<IScene>()>> factories_;
    
    std::unordered_map<std::string, std::string> pairs;

    bool shouldEnd = false;

    public:
        template<typename T>
        void addScene(const std::string& name) {
            static_assert(std::is_base_of<IScene, T>::value, "T must inherit from IScene");
            factories_[name] = [this](){return std::make_unique<T>(*this);};
        }

        void addWindow(const std::string& name, std::unique_ptr<Window>&& window);

        void attachSceneToWindow(const std::string& scene, const std::string& window);

        std::optional<std::reference_wrapper<Window>> getWindow(const std::string& name);

        void run();
};