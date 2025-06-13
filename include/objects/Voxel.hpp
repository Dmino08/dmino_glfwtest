#pragma once

#include "graphics/core/Mesh.hpp"
#include "utils/Transform.hpp"


class Voxel {
    Mesh mesh_;

    void generateMesh();

    public:
        util::Transform transform;
        Voxel();
        Voxel(const glm::vec3& position);
        void draw(GLenum mode = GL_TRIANGLES) const;

};