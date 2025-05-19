#pragma once

#include "graphics/core/Mesh.hpp"
#include "utils/Transform.hpp"

class Voxel {
    static Mesh mesh_;

    static void generateMesh();
    static bool meshGenerated_;

    public:
        util::Transform transform;
        Voxel();
        Voxel(const glm::vec3& position);
        void draw();

};