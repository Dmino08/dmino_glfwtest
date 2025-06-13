#pragma once

#pragma once

#include "engine/IScene.hpp"

#include "typedefs.hpp"

class Mesh;

class VoidScene_sc : public IScene
{
public:
    VoidScene_sc();
    ~VoidScene_sc();

    void init(Engine& engine, Window& window) override;
    void input(InputManager& input, float delta) override;
    void update(float delta) override;
    void draw() override;
    void onClose() override;

private:
    Window* window_;
    Engine* engine_;
    core::Time* time_;
    u_ptr<Mesh> mesh_;

};