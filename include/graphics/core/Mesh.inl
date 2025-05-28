#pragma once

#include <iostream>

template <typename V>
void Mesh::create(const MeshData<V>& data, GLenum usage) {

    if (data.vertices.empty()) return;

    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
        vao_ = 0;
        vbo_ = 0;
        ebo_ = 0;
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(V), data.vertices.data(), usage);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint), data.indices.data(), usage);
    
    for (const VertexAttribute& attr : V::attrs)
    {
        glVertexAttribPointer(attr.index, 
                              attr.size, 
                              attr.type, 
                              attr.normalized, 
                              attr.stride,
                              reinterpret_cast<void*>(attr.offset));
        glEnableVertexAttribArray(attr.index);
    }

    glBindVertexArray(0);

    verticeCount_ = data.vertices.size();
    indiceCount_ = data.indices.size();
}
