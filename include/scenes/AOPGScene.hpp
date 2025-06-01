#pragma once

#include "engine/Engine.hpp"
#include "engine/IScene.hpp"
#include "assets/Assets.hpp"
#include "typedefs.hpp"


#include <memory>

class Window;
class InputManager;
class Shader;
class Voxel;
class Camera;


class AOPGScene : public IScene {
    public:
        Assets& assets;
        AOPGScene(Engine& engine) : IScene(engine), assets(engine.getAssets()) {}

        InputManager* input = nullptr;
        Window* window;

        std::unique_ptr<Camera> camera;

        asset<Shader> shader;
        uint ubo_buffer;
        
        std::unique_ptr<Voxel> voxel;


        float camera_speed = 10.f;
        float fpsTimer = 0.0f;



        void init(Window& window) override;
        void update(float delta) override;
        void draw() override;
};