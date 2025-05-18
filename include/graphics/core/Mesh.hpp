#pragma once


#include <glad/glad.h>
#include "utils/Buffer.hpp"


struct Vertex {
    float position[3];
    float normal[3];
    float tex_coord[2];
};

struct MeshData {
    util::Buffer<Vertex> vertices;
    util::Buffer<uint> indices;
};


class Mesh {
    uint vao_, vbo_, ebo_;

    MeshData data_;
    uint verticeCount_;
    uint indiceCount_;

    public:
        Mesh();
        Mesh(MeshData data);
        void draw();

};