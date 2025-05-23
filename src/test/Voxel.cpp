#include "test/Voxel.hpp"


Mesh Voxel::mesh_ = Mesh();
bool Voxel::meshGenerated_ = false;


void Voxel::generateMesh() {
    
    // Vertex data
    std::vector<Vertex> cubeVertices = {
        // ЗАДНЯЯ грань (нормаль: -Z)
        {{-0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f},  {0.0f, 1.0f}},

        // ПЕРЕДНЯЯ грань (нормаль: +Z)
        {{-0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  { 0.0f,  0.0f,  1.0f},  {0.0f, 1.0f}},

        // ЛЕВАЯ грань (нормаль: -X)
        {{-0.5f, -0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},

        // ПРАВАЯ грань (нормаль: +X)
        {{ 0.5f, -0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},

        // НИЖНЯЯ грань (нормаль: -Y)
        {{-0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},

        // ВЕРХНЯЯ грань (нормаль: +Y)
        {{-0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  { 0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
    };
    // Indices data
    std::vector<uint> cubeIndices = {
        0, 2, 1,  0, 3, 2,      // задняя
        4, 5, 6,  4, 6, 7,      // передняя
        8, 10, 9,  8, 11,10,    // левая
        12,14,13, 12,15,14,     // правая
        16,17,18, 16,18,19,     // нижняя
        20,21,22, 20,22,23      // верхняя
    };

    MeshData meshData{std::move(cubeVertices), std::move(cubeIndices)}; 
    
    Voxel::mesh_.createMesh(meshData);
    meshGenerated_ = true;
}

Voxel::Voxel() : transform(glm::vec3(0.0f)) {
    if (!meshGenerated_)
    {
        Voxel::generateMesh();
    }
}

Voxel::Voxel(const glm::vec3& position) : transform(position) {
    if (!meshGenerated_)
    {
        Voxel::generateMesh();
    }
    
}


void Voxel::draw() {
    Voxel::mesh_.draw();
}