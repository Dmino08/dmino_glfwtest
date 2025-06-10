#pragma once


#include <string>

#include "engine/Engine.hpp"
#include "window/Window.hpp"
#include "window/Camera.hpp"
#include "window/InputManager.hpp"

class IScene {
    public:
        IScene();
        virtual ~IScene() = 0;

        virtual void init(Engine& engine, Window& window) {}
        virtual void preUpdate(float delta) {}
        virtual void update(float delta) {} 
        virtual void input(InputManager& input, float delta) {}
        virtual void draw() {}
        virtual void afterUpdate(float delta) {}
        virtual void onClose() {}
};

inline IScene::IScene() {}
inline IScene::~IScene() {}