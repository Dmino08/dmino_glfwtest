#include "graphics/core/UVRegion.hpp"



UVRegion::UVRegion() 
    : u_offset_(0.0f), 
      v_offset_(0.0f), 
      u_size_(1.0f), 
      v_size_(1.0f) 
  {}

UVRegion::UVRegion(const UVRegion& other) 
    : u_offset_(other.u_offset_), 
      v_offset_(other.v_offset_), 
      u_size_(other.u_size_), 
      v_size_(other.v_size_) 
{
}

UVRegion& UVRegion::operator=(const UVRegion& other) {
    if (this != &other) {
        u_offset_ = other.u_offset_;
        v_offset_ = other.v_offset_;
        u_size_ = other.u_size_;
        v_size_ = other.v_size_;
    }
    return *this;
}

void UVRegion::setRegion(int width, 
                         int height, 
                         int x_offset, 
                         int y_offset, 
                         int x_size, 
                         int y_size) 
{
    if (width >= 0 && height >= 0) {
        u_offset_ = float(x_offset) / float(width);
        v_offset_ = float(y_offset) / float(height);

        u_size_ = float(x_size) / float(width);
        v_size_ = float(y_size) / float(height); 
    }
}

const glm::vec2 UVRegion::getUVMin() const {
    return glm::vec2(u_offset_, v_offset_);
}
const glm::vec2 UVRegion::getUVMax() const {
    return glm::vec2(u_offset_ + u_size_, v_offset_ + v_size_);
}
