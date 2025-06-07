#pragma once


#include "engine/IScene.hpp"
#include "engine/Engine.hpp"

#include "window/Camera.hpp"
#include "graphics/core/Sprite.hpp"
#include "graphics/core/Shader.hpp"
#include "graphics/render/FrameBuffer.hpp"
#include "objects/Voxel.hpp"
#include "graphics/core/CubeMap.hpp"
#include "graphics/core/Model.hpp"

#include "typedefs.hpp"

class Assets;
class Engine;
class InputManager;
class Image;
class Shader;


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

        u_ptr<modload::Model> model;
        size_t instance_size = 0;
        size_t instance_side = 0;
        std::vector<glm::vec3> instance_pos;
        bool is_instance_draw = true;

        float camera_speed = 10.f;
        float fpsTimer = 0.0f;
        bool vsync = false;
        bool onFlashLight = true;

        void init(Window& window) override;

        void update(float delta) override;

        void draw() override;

};