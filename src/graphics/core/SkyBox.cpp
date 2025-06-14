#include "graphics/core/SkyBox.hpp"

SkyBox::SkyBox() {}
SkyBox::~SkyBox() {}

SkyBox::SkyBox(const SkyBox& other) {
    texture = other.texture;
}
SkyBox& SkyBox::operator=(const SkyBox& other) {
    if (this != &other) {
        texture = other.texture;
    }    
    texture = other.texture;
    return *this;
}

SkyBox::SkyBox(SkyBox&& other) noexcept {
    texture = std::move(other.texture);
}
SkyBox& SkyBox::operator=(SkyBox&& other) noexcept {
    if (this != &other) {
        texture = other.texture;
    }        
    texture = std::move(other.texture);
    return *this;
}


void SkyBox::clear() {
    texture.clear();
}

void SkyBox::create(Image images[6], TextureParams params) {
    clear();
    texture.generateTexture(params);
    for (size_t i = 0; i < 6; i++)
    {
        texture.texImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, images[i].getWidth(), images[i].getHeight(), images[i].getData(), params);
    }
}