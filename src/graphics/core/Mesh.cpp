#include "graphics/core/Mesh.hpp"

Mesh::Mesh() : vao_(0), vbo_(0), ebo_(0), data_(MeshData{}) {}


void Mesh::createMesh(const MeshData& data, GLenum usage) {
    
    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
    }
    
    data_ = data;

    // Generating Vertex Array and its Buffers
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
    // Binding Vertex Array
    glBindVertexArray(vao_);
    
    // Binding VBO Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, data_.vertices.size() * sizeof(Vertex), data_.vertices.data(), usage);
    // Binding EBO Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_.indices.size() * sizeof(Vertex), data_.indices.data(), usage);

    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Normal Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // Texture Coord Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv_coord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    
    verticeCount_ = data.vertices.size();
    indiceCount_ = data.indices.size();

    
}

void Mesh::draw() {
    glBindVertexArray(vao_);

    if (ebo_ != 0) {
        glDrawElements(GL_TRIANGLES, indiceCount_, GL_UNSIGNED_INT, 0);
    }
    else if (vbo_ != 0) {
        glDrawArrays(GL_TRIANGLES, 0, verticeCount_);
    }

    glBindVertexArray(0);
}