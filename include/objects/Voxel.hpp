#pragma once

#include "graphics/core/Mesh.hpp"
#include "utils/Transform.hpp"

class GLFWwindow;

class Voxel {
    static Mesh mesh_;
    static GLFWwindow* current_context_;

    static void generateMesh();

    public:
        util::Transform transform;
        Voxel();
        Voxel(const glm::vec3& position);
        void draw(GLenum mode = GL_TRIANGLES) const;

};