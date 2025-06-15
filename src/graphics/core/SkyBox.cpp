#include "graphics/core/SkyBox.hpp"

SkyBox::SkyBox() {}
SkyBox::~SkyBox() {}

SkyBox::SkyBox(SkyBox&& other) noexcept {
    texture = std::move(other.texture);
}
SkyBox& SkyBox::operator=(SkyBox&& other) noexcept {
    if (this != &other) {
        texture = std::move(other.texture);
    }        
    return *this;
}


void SkyBox::clear() {
    texture.clear();
}

void SkyBox::create(Image images[6], const TextureParams& params) {
    clear();
    Texture::activeUnit(Texture::getFreeUnit());
    texture.bind();
    texture.generateTexture(params);
    for (size_t i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, params.internal_format, images[i].getWidth(), images[i].getHeight(), 0, images[i].getFormat(),
            params.type, images[i].getData()); 
    }
}

void SkyBox::create(std::string paths[6], const TextureParams& params) {
    Image images[6];
    images[0].load(paths[0]);
    images[1].load(paths[1]);
    images[2].load(paths[2]);
    images[3].load(paths[3]);
    images[4].load(paths[4]);
    images[5].load(paths[5]);

    create(images, params);
}