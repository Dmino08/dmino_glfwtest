#pragma once

#include "engine/IScene.hpp"

#include "typedefs.hpp"

class Camera;
class Shader;
class Texture;
class Voxel;
class Sprite;


class ShadowMap_sc : public IScene
{
public:
    ShadowMap_sc();
    ~ShadowMap_sc();

    void init(Engine& engine, Window& window) override;
    void input(InputManager& input, float delta) override;
    void draw() override;
private:
    
    Window* window_;
    core::Time* time_;

    // CAMERA STUFF
    u_ptr<Camera> camera_;
    float camera_speed_ = 15.0f;

    // SHADER STUFF
    u_ptr<Shader> sh_main_;

    // BOX STUFF
    u_ptr<Texture> box_texture_;
    u_ptr<Voxel> box_;

    // FLOOR STUFF
    u_ptr<Texture> floor_texture_;
    float floor_size_ = 15'000.0f;
    u_ptr<Sprite> floor_;
};