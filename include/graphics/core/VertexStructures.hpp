#pragma once

#include "graphics/core/Mesh.hpp"

struct ScreenVertex {
    glm::vec2 position;
    glm::vec2 uv_coord;

    static const std::array<VertexAttribute, 2> attrs;
};