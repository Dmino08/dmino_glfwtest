#include "graphics/core/Sprite.hpp"
#include "graphics/core/VertexStructures.hpp"
#include <array>

Sprite::Sprite()
    : region_(),
      mesh_(),
      color_(1.0f),
      width_(1),
      height_(1)
{}

void Sprite::setTextureSize(uint width, uint height) {
    width_ = width;
    height_ = height;
}

void Sprite::setRegion(int x_offset, int y_offset, int x_size, int y_size) {
    region_.setRegion(width_, 
                      height_, 
                      x_offset,
                      y_offset,
                      x_size,
                      y_size);
}

const UVRegion& Sprite::getRegion() const {
    return region_;
}

void Sprite::generate() {
    
    glm::vec2 uv_min = region_.getUVMin();
    glm::vec2 uv_max = region_.getUVMax();

    std::array<SimpleVertex, 12> vertices = {{
    //        position              normal            uv_coord
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {uv_min.x, uv_min.y}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {uv_max.x, uv_min.y}}, 
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {uv_max.x, uv_max.y}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {uv_min.x, uv_max.y}}

    }};
    std::array<uint, 6> indices = {
        0, 1, 2,
        2, 3, 0
    };    
    mesh_.create(vertices.size(), indices.size());
    mesh_.bind();
    mesh_.setBuffer(GL_ARRAY_BUFFER, vertices.size() * sizeof(SimpleVertex), vertices.data(), GL_STATIC_DRAW);
    mesh_.setBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    mesh_.setAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                reinterpret_cast<void*>(offsetof(SimpleVertex, position)));

    mesh_.setAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                reinterpret_cast<void*>(offsetof(SimpleVertex, normal)));

    mesh_.setAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                reinterpret_cast<void*>(offsetof(SimpleVertex, uv_coord)));
    mesh_.unbind();

}

void Sprite::draw() const{
    mesh_.draw();
}