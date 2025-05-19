#include "graphics/core/Sprite.hpp"

Sprite::Sprite()
    : region_(),
      mesh_(),
      color_(1.0f),
      texture_(nullptr)
{}

void Sprite::setTexture(Texture* texture) {
    texture_ = texture;
}

void Sprite::setRegion(int x_offset, int y_offset, int x_size, int y_size) {
    if (texture_ != nullptr) {
    region_.setRegion(texture_->getWidth(), 
                      texture_->getHeight(), 
                      x_offset,
                      y_offset,
                      x_size,
                      y_size);
    }
}

const UVRegion& Sprite::getRegion() const {
    return region_;
}

void Sprite::generate() {
    
    glm::vec2 uv_min = region_.getUVMin();
    glm::vec2 uv_max = region_.getUVMax();

    util::Buffer<Vertex> vertices = {
    //        position              normal            uv_coord
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {uv_min.x, uv_min.y}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {uv_max.x, uv_min.y}}, 
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {uv_max.x, uv_max.y}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {uv_min.x, uv_max.y}}

    };
    util::Buffer<uint> indices = {
        0, 1, 2,
        2, 3, 0
    };    
    mesh_.createMesh(MeshData({vertices, indices}));

}

void Sprite::draw() {
    mesh_.draw();
}