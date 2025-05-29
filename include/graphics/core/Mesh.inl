#pragma once

#include <iostream>
#include "core/Logger.hpp"

#ifdef DEBUG_MODE
    extern int generatedMeshes;
#endif

template <typename V>
void Mesh::create(const MeshData<V>& data, GLenum usage) {

    if (data.vertices.empty()) return;

    clear();

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

    #ifdef DEBUG_MODE
        generatedMeshes++;
        core::logger.log(core::Logger::INFO, "Mesh " + std::to_string(generatedMeshes) + " is generated");
    #endif
}
