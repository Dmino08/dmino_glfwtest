#include "graphics/core/Mesh.hpp"

#include <cstddef>

#ifdef DEBUG_MODE
    int generated_meshes = 0;
#endif

const std::array<VertexAttribute, 3> SimpleVertex::attrs = {{
    {0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), offsetof(SimpleVertex, position)},
    {1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), offsetof(SimpleVertex, normal)},
    {2, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), offsetof(SimpleVertex, uv_coord)}
}};

int Mesh::draw_calls_ = 0; 

Mesh::Mesh() : vao_(0), vbo_(0), ebo_(0), vertice_count_(0), indice_count_(0) {}

Mesh::Mesh(Mesh&& other) noexcept {
    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ebo_ = other.ebo_;
    vertice_count_ = other.vertice_count_;
    indice_count_ = other.indice_count_;

    other.vao_ = 0;
    other.vbo_ = 0;
    other.ebo_ = 0;
    other.vertice_count_ = 0;
    other.indice_count_ = 0;
}
Mesh& Mesh::operator= (Mesh&& other) noexcept {
    if (this != &other) {
            clear();

            vao_ = other.vao_;
            vbo_ = other.vbo_;
            ebo_ = other.ebo_;
            vertice_count_ = other.vertice_count_;
            indice_count_ = other.indice_count_;

            other.vao_ = 0;
            other.vbo_ = 0;
            other.ebo_ = 0;
            other.vertice_count_ = 0;
            other.indice_count_ = 0;
        }
    return *this;
}

Mesh::~Mesh() {
    clear();  
}

void Mesh::clear() {
    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
        vao_ = 0;
        vbo_ = 0;
        ebo_ = 0;
    }       
}

void Mesh::draw(GLenum mode) const {
    glBindVertexArray(vao_);

    if (vertice_count_ != 0) {
        if (indice_count_ != 0) {
            glDrawElements(mode, indice_count_, GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(mode, 0, vertice_count_);
        }
        draw_calls_++;
    }

    glBindVertexArray(0);
}

void Mesh::drawInstances(GLsizei instance_count, GLenum mode) const {
    glBindVertexArray(vao_);

    if (vertice_count_ != 0) {
        if (indice_count_ != 0) {
            glDrawElementsInstanced(mode, indice_count_, GL_UNSIGNED_INT, 0, instance_count);
        }
        else {
            glDrawArraysInstanced(mode, 0, vertice_count_, instance_count);
        }
        draw_calls_++;
    }

    glBindVertexArray(0);
}

int Mesh::getDrawCalls() {
    return draw_calls_;
}

void Mesh::clearDrawCalls() {
    draw_calls_ = 0;
}