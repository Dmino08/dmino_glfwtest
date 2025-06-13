#pragma once

#include "engine/IScene.hpp"

#include "graphics/core/Sprite.hpp"
#include "objects/Voxel.hpp"
#include "graphics/render/FrameBuffer.hpp"
#include "graphics/core/Model.hpp"

#include "typedefs.hpp"

#include <array>
#include <glm/glm.hpp>

class Camera;
class Shader;
class Texture;
class Voxel;
class Sprite;
class Mesh;

struct DepthPointLight
{
    glm::vec3 pos;
    float far_plane;
    glm::mat4 shad_proj;
    std::array<glm::mat4, 6> shad_transforms;
    void setUpMatrix();
};

class PointShadows_sc : public IScene
{
public:
    PointShadows_sc();
    ~PointShadows_sc();

    void init(Engine& engine, Window& window) override;
    void input(InputManager& input, float delta) override;
    void update(float delta) override;
    void draw() override;
    void onClose() override;
    
    void renderScene(Shader& shader, bool is_depth = true);


private:
    Window* window_;
    Engine* engine_;
    core::Time* time_;

    // CAMERA STUFF
    u_ptr<Camera> camera_;
    float camera_speed_ = 15.0f;    

    // SHADER STUFF
    u_ptr<Shader> sh_main_;
    u_ptr<Shader> sh_shadow_;

    // FLOOR AND WALLS STUFF
    u_ptr<Texture> simple_texture_;
    float side_size_ = 15.0f;
    std::array<Sprite, 6> sides_;

    // BOX STUFF
    u_ptr<Texture> box_texture_;
    std::array<Voxel, 10> boxes_;

    // MODEL STUFF
    u_ptr<modload::Model> model_;

    // ROTATION STUFF
    float rotation_ = 0.0f;

    // FBO STUFF
    FrameBuffer fbo_;

    // LIGHT STUFF
    DepthPointLight dpl_;
    int SHADOW_W_H = 0;

};