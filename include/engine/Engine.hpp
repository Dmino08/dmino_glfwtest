#pragma once

#include "assets/Assets.hpp"
#include "core/Time.hpp"
#include "engine/EventSystem.hpp"

#include <functional>


class Window;
class IScene;

class Engine {
    Assets assets_;
    core::Time time_;
    

    std::unordered_map<std::string, 
                       std::pair<std::unique_ptr<Window>, 
                       std::unique_ptr<IScene>>> pairs_;
    std::unordered_map<std::string, std::function<std::unique_ptr<IScene>()>> factories_;


    bool should_end_ = false;

    public:
        EventSystem events;
        ~Engine();
        template<typename T>
        void addScene(const std::string& name) {
            static_assert(std::is_base_of<IScene, T>::value, "T must inherit from IScene");
            factories_[name] = [](){return std::make_unique<T>();};
        }

        void addWindow(const std::string& name, std::unique_ptr<Window>&& window);

        void attachSceneToWindow(const std::string& scene, const std::string& window);

        std::optional<std::reference_wrapper<Window>> getWindow(const std::string& name);

        Assets& getAssets();
        core::Time& getTime();

        void run();
};