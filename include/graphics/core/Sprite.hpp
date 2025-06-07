#pragma once

#include <glm/glm.hpp>

#include "graphics/core/UVRegion.hpp"
#include "graphics/core/Mesh.hpp"

#include "utils/Transform.hpp"

#include "typedefs.hpp"

class Sprite {
    
    UVRegion region_;
    Mesh mesh_;
    glm::vec3 color_;
    uint width_, height_;

    public:
        util::Transform transform;
        
        Sprite();
        void setTextureSize(uint width, uint height);
        void setRegion(int x_offset, int y_offset, int x_size, int y_size);

        const UVRegion& getRegion() const;

        void generate();
        void draw() const;
};