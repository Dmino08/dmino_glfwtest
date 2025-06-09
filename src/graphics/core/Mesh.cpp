#include "graphics/core/Mesh.hpp"
#include "core/Logger.hpp"

#include <cstddef>

#ifdef DEBUG_MODE
    int generated_meshes = 0;
#endif

int Mesh::draw_calls_ = 0; 

Mesh::Mesh() : vao_(0), buffers_(), vertice_count_(0), indice_count_(0), buffer_count_(0) {}

Mesh::Mesh(Mesh&& other) noexcept {
    vao_ = other.vao_;
    buffers_ = other.buffers_;
    vertice_count_ = other.vertice_count_;
    indice_count_ = other.indice_count_;


    other.vao_ = 0;
    other.buffers_ = std::vector<GLuint>();
    other.vertice_count_ = 0;
    other.indice_count_ = 0;
}
Mesh& Mesh::operator= (Mesh&& other) noexcept {
    if (this != &other) {
            clear();

            vao_ = other.vao_;
            buffers_ = other.buffers_;
            vertice_count_ = other.vertice_count_;
            indice_count_ = other.indice_count_;

            other.vao_ = 0;
            other.buffers_ = std::vector<GLuint>();
            other.vertice_count_ = 0;
            other.indice_count_ = 0;
            
        }
    return *this;
}

Mesh::~Mesh() {
    clear();  
}

void Mesh::clear() {
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }

    if (!buffers_.empty()) {
        glDeleteBuffers(static_cast<GLsizei>(buffers_.size()), buffers_.data());
        buffers_.clear();
    }

    vertice_count_ = 0;
    indice_count_ = 0;   
} 

void Mesh::create(uint vertice_count, uint indice_count) {
    vertice_count_ = vertice_count;
    indice_count_ = indice_count;
    
    glGenVertexArrays(1, &vao_);

    #ifdef DEBUG_MODE
        generated_meshes++;
        core::logger.log(core::Logger::INFO, "Mesh " + std::to_string(generated_meshes) + " is generated");
    #endif
}
void Mesh::bind() {
    glBindVertexArray(vao_);
}

void Mesh::setBuffer(GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, usage);
    buffers_.push_back(buffer);
    buffer_count_++;
}

void Mesh::setAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer, bool is_divisor) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
    if (is_divisor)
        glVertexAttribDivisor(index, 1);
}

void Mesh::unbind() {
    glBindVertexArray(0);
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