#include "graphics/core/VertexStructures.hpp"

const std::array<VertexAttribute, 2> ScreenVertex::attrs = {{
    {0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), offsetof(ScreenVertex, position)},
    {1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), offsetof(ScreenVertex, uv_coord)}
}};