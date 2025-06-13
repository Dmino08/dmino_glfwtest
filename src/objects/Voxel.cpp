#include "objects/Voxel.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/core/VertexStructures.hpp"


void Voxel::generateMesh() {
    
    // Vertex data
    std::vector<SimpleVertex> cubeVertices = {
        // ЗАДНЯЯ грань (нормаль: -Z)
        {{-0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {0.0f, 1.0f}},

        // ПЕРЕДНЯЯ грань (нормаль: +Z)
        {{-0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {0.0f, 1.0f}},

        // ЛЕВАЯ грань (нормаль: -X)
        {{-0.5f, -0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},

        // ПРАВАЯ грань (нормаль: +X)
        {{ 0.5f, -0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},

        // НИЖНЯЯ грань (нормаль: -Y)
        {{-0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},

        // ВЕРХНЯЯ грань (нормаль: +Y)
        {{-0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
    };
    // Indices data
    std::vector<uint> cubeIndices = {
        0, 2, 1,  0, 3, 2,      // задняя
        4, 5, 6,  4, 6, 7,      // передняя
        8, 10, 9,  8, 11,10,    // левая
        12,14,13, 12,15,14,     // правая
        16,17,18, 16,18,19,     // нижняя
        20,21,22, 20,22,23      // верхняя
    };
    
    mesh_.create(cubeVertices.size(), cubeIndices.size());
    mesh_.bind();
    mesh_.setBuffer(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(SimpleVertex), cubeVertices.data(), GL_STATIC_DRAW);
    mesh_.setBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(uint), cubeIndices.data(), GL_STATIC_DRAW);

    mesh_.setAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                reinterpret_cast<void*>(offsetof(SimpleVertex, position)));

    mesh_.setAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                reinterpret_cast<void*>(offsetof(SimpleVertex, normal)));

    mesh_.setAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                reinterpret_cast<void*>(offsetof(SimpleVertex, uv_coord)));
    mesh_.unbind();
}

Voxel::Voxel() : transform(glm::vec3(0.0f)) {
    generateMesh();
}

Voxel::Voxel(const glm::vec3& position) : transform(position) {
    generateMesh();
}


void Voxel::draw(GLenum mode) const {
    mesh_.draw(mode);
}
