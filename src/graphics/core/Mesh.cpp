#include "graphics/core/Mesh.hpp"
#include <cstddef>

const std::array<VertexAttribute, 3> SimpleVertex::attrs = {{
    {0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), offsetof(SimpleVertex, position)},
    {1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), offsetof(SimpleVertex, normal)},
    {2, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), offsetof(SimpleVertex, uv_coord)}
}};


Mesh::Mesh() : vao_(0), vbo_(0), ebo_(0), verticeCount_(0), indiceCount_(0) {}

Mesh::~Mesh() {
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

void Mesh::draw() const {
    glBindVertexArray(vao_);

    if (verticeCount_ != 0) {
        if (indiceCount_ != 0) {
            glDrawElements(GL_TRIANGLES, indiceCount_, GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, verticeCount_);
        }
        
    }

    glBindVertexArray(0);
    
}