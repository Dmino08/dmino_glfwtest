#pragma once

#include "graphics/core/Shader.hpp"
#include "graphics/core/Mesh.hpp"

#include "window/Window.hpp"
#include "window/Camera.hpp"

class Canvas {
    Shader ui_shader_;
    Camera ui_camera_;  

    Mesh mesh_;

    std::vector<Vertex> vertices_;
    std::vector<uint> indices_;

    size_t vertex_count_;
    size_t indices_count_;

    Window& window_;

    public:
        Canvas(Window& window);
        void init();
        void begin();
        void end();
};