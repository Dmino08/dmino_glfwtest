#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include "typedefs.hpp"



struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv_coord;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
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