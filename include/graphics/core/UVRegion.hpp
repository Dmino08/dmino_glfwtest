#pragma once

#include <utility>
#include <glm/glm.hpp>

class UVRegion {
    float u_offset_;
    float v_offset_;

    float u_size_;
    float v_size_;

    public:
        UVRegion();
        UVRegion(const UVRegion& other);
        UVRegion& operator=(const UVRegion& other);

        void setRegion(int width, int height, int x_offset, int y_offset, int x_size, int y_size);
        
        const glm::vec2 getUVMin() const;
        const glm::vec2 getUVMax() const;
};