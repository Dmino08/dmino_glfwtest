#pragma once


#include "engine/IScene.hpp"
#include "engine/Engine.hpp"

#include "window/Camera.hpp"
#include "graphics/core/Sprite.hpp"
#include "graphics/core/Shader.hpp"
#include "graphics/render/FrameBuffer.hpp"
#include "test/Voxel.hpp"
#include "graphics/core/CubeMap.hpp"


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

        u_ptr<FrameBuffer> fbo;

        u_ptr<Shader> mult_shader;
        u_ptr<Shader> screen_shader;
        u_ptr<Shader> skybox_shader;
        u_ptr<Shader> reflect_shader;

        asset<Image> image0;

        u_ptr<Sprite> floor;
        u_ptr<CubeMap> cube_map;

        u_ptr<Voxel> skybox;
        u_ptr<Voxel> reflectbox;

        Texture texture0;

        float camera_speed = 10.f;
        bool vsync = false;
        bool onFlashLight = true;

        void init(Window& window) override;

        void update(float delta) override;

        void draw() override;
};