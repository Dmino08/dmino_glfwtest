#pragma once

#include "engine/IScene.hpp"

#include "typedefs.hpp"

class InputManager;
class Camera;
class Shader;
class Sprite;
class Voxel;
class Texture;

class ShadowMap_sc : public IScene 
{   
public:
    ShadowMap_sc(Engine& engine) : IScene(engine) {}
    void init(Window& window) override;
    void update(float delta) override;
    void draw() override;
private:
    Window* window_;
    InputManager* input_;

    u_ptr<Camera> camera_;

    u_ptr<Shader> sh_light_;
    u_ptr<Shader> sh_depth_;
    u_ptr<Shader> sh_fbo_;
    
    u_ptr<Texture> floor_texture_;
    float floor_size_ = 15'000.0f;
    u_ptr<Sprite> floor_;

    u_ptr<Texture> box_texture_;
    u_ptr<Voxel> box_;

    float camera_speed_ = 10.0f;

};