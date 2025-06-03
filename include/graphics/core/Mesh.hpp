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
    uint vertice_count_, indice_count_;

    static int draw_calls_;

    public:
        Mesh();
        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;
        ~Mesh();
        void clear();

        template <typename V>
        void create(const MeshData<V>& data, GLenum usage = GL_STATIC_DRAW);

        void draw(GLenum mode = GL_TRIANGLES) const;
        void drawInstances(GLsizei instance_count, GLenum mode = GL_TRIANGLES) const;
        
        static int getDrawCalls();
        static void clearDrawCalls();
};

#include "graphics/core/Mesh.inl"