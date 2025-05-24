#pragma once


#include "engine/IScene.hpp"

#include <memory>
#include <vector>

template <typename T>
using u_ptr = std::unique_ptr<T>;



class MainScene : public IScene {

    public:
        MainScene();

        int init() override;

        void update(float deltaTime) override;

};