#pragma once

#include "engine/IScene.hpp"

#include "typedefs.hpp"

#include "graphics/core/Model.hpp"

class Camera;
class Shader;
class Texture;
class Voxel;
class Sprite;
class Mesh;

class ShadowMap_sc : public IScene
{
public:
    ShadowMap_sc();
    ~ShadowMap_sc();

    void init(Engine& engine, Window& window) override;
    void input(InputManager& input, float delta) override;
    void update(float delta) override;
    void draw() override;

    void renderScene(Shader& shader, bool is_depth = true);
private:
    Window* window_;
    core::Time* time_;

    // CAMERA STUFF
    u_ptr<Camera> camera_;
    float camera_speed_ = 15.0f;

    // SHADER STUFF
    u_ptr<Shader> sh_main_;
    u_ptr<Shader> sh_depth_;
    u_ptr<Shader> sh_simple_;

    // BOX STUFF
    u_ptr<Texture> box_texture_;
    u_ptr<Voxel> box_;

    // FLOOR STUFF
    u_ptr<Texture> floor_texture_;
    float floor_size_ = 15'000.0f;
    u_ptr<Sprite> floor_;

    // LIGHT STUFF
    glm::vec3 light_dir_;
    glm::vec3 light_pos_;
    float light_distance = 10.0f;
    bool shadow_on_ = true;

    // DEPTH STUFF
    uint SHADOW_WIDTH, SHADOW_HEIGHT;
    uint depth_fbo_;
    uint depth_map_;
    u_ptr<Mesh> depth_mesh_;

    // MODEL STUFF
    u_ptr<Texture> model_texture_;
    u_ptr<modload::Model> model_;
    uint side_size_ = 10;
    float rotation = 0.0f;


};