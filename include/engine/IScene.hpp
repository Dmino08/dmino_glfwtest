#pragma once



class IScene {
    public:
        virtual ~IScene() = 0;

        virtual int init() = 0;

        virtual void preUpdate() {}

        virtual void update(float deltaTime) = 0;

        virtual void afterUpdate() {}
};

inline IScene::~IScene() {}