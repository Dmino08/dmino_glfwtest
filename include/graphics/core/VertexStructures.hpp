#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct SimpleVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv_coord;

};

struct ScreenVertex {
    glm::vec2 position;
    glm::vec2 uv_coord;

};