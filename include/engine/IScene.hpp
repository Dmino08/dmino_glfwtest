#pragma once

class Engine;

class IScene {
    protected:
        Engine& engine_;
    public:
        IScene(Engine& engine);
        virtual ~IScene() = 0;
        virtual void init() {}
        virtual void preUpdate(float delta) {}
        virtual void update(float delta) {} 
        virtual void afterUpdate(float delta) {}
};

inline IScene::IScene(Engine& engine)  : engine_(engine) {}
inline IScene::~IScene() {}