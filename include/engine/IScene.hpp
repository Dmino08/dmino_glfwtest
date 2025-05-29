#pragma once


#include <string>

class Engine;
class Window;

class IScene {
    protected:
        Engine& engine_;
    public:
        IScene(Engine& engine);

        virtual ~IScene() = 0;
        virtual void init(Window& window) {}
        virtual void preUpdate(float delta) {}
        virtual void update(float delta) {} 
        virtual void draw() {}
        virtual void afterUpdate(float delta) {}
};

inline IScene::IScene(Engine& engine)  : engine_(engine) {}
inline IScene::~IScene() {}