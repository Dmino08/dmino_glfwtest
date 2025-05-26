#pragma once


#include "engine/IScene.hpp"
#include "engine/Engine.hpp"

#include "window/Camera.hpp"
#include "graphics/core/Sprite.hpp"
#include "graphics/core/Shader.hpp"


class Assets;
class Engine;
class InputManager;
class Image;
class Shader;

template<typename T>
using u_ptr = std::unique_ptr<T>;

#define makeU std::make_unique
#define makeS std::make_shared

class MainScene : public IScene {
    public:
        Assets& assets;
        MainScene(Engine& engine) : IScene(engine), assets(engine.getAssets()) {}

        InputManager* input = nullptr;
        Window* window;

        u_ptr<Camera> camera;

        u_ptr<Shader> mult_shader;

        asset<Image> image0;

        u_ptr<Sprite> floor;

        Texture texture0;

        float camera_speed = 10.f;
        bool vsync = true;
        bool onFlashLight = true;

        void init(Window& window) override;

        void update(float delta) override;
};