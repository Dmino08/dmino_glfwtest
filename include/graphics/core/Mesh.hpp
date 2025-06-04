#pragma once

#include "typedefs.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


class Mesh {
    GLuint vao_;
    std::vector<GLuint> buffers_;
    uint vertice_count_, indice_count_, buffer_count_;

    static int draw_calls_;

    public:
        Mesh();
        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;
        ~Mesh();
        void clear();

        void create(uint vertice_count, uint indice_count);
        void bind();
        void setBuffer(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
        void setAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
        void unbind();

        void draw(GLenum mode = GL_TRIANGLES) const;
        void drawInstances(GLsizei instance_count, GLenum mode = GL_TRIANGLES) const;
        
        static int getDrawCalls();
        static void clearDrawCalls();
};
