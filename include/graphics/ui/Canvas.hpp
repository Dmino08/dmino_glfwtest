#pragma once

#include "graphics/core/Shader.hpp"
#include "graphics/core/Mesh.hpp"
#include "window/Camera.hpp"

class Canvas {
    
    Window& window_;

    std::unique_ptr<Shader> shader_;
    Mesh mesh_;
    glm::mat4 model;

    Camera ui_camera_;
    
    MeshData data_;

    size_t verticeCount_;
    size_t indiceCount_;

    public:
        Canvas(Window& window);
        void init();
        void begin();
        void end();

        void drawRect(float x, float y, float width, float height, const glm::vec3& color);
};