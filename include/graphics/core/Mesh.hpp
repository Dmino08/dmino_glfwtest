#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>

#include "utils/Buffer.hpp"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv_coord;
};

struct MeshData {
    util::Buffer<Vertex> vertices;
    util::Buffer<uint> indices;
};


class Mesh {
    uint vao_, vbo_, ebo_;

    MeshData data_;
    uint verticeCount_;
    uint indiceCount_;

    public:
        Mesh();
        void createMesh(const MeshData& data, GLenum usage = GL_STATIC_DRAW);
        void draw();

};