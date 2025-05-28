#pragma once

#include "typedefs.hpp"

#include <glad/glad.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>

struct VertexAttribute {
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    size_t offset;
};

struct SimpleVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv_coord;

    static const std::array<VertexAttribute, 3> attrs;
};

template <typename V>
struct MeshData {
    std::vector<V> vertices;
    std::vector<uint> indices;
};

class Mesh {
    GLuint vao_, vbo_, ebo_;
    uint verticeCount_, indiceCount_;
    public:
        Mesh();
        ~Mesh();

        template <typename V>
        void create(const MeshData<V>& data, GLenum usage = GL_STATIC_DRAW);

        void draw() const;
};

#include "graphics/core/Mesh.inl"