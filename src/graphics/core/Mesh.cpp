#include "graphics/core/Mesh.hpp"

Mesh::Mesh(MeshData data) : vao_(0), vbo_(0), ebo_(0), data_(data){
    // Generating Vertex Array and its Buffers
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
    // Binding Vertex Array
    glBindVertexArray(vao_);
    // Binding VBO Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, data_.vertices.getByteSize(), data_.vertices.begin(), GL_STATIC_DRAW);
    // Binding EBO Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_.indices.getByteSize(), data_.indices.begin(), GL_STATIC_DRAW);

    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Normal Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // Texture Coord Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    verticeCount_ = data.vertices.getSize();
    indiceCount_ = data.indices.getSize();
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