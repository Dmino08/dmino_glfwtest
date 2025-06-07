#include "graphics/core/Texture.hpp"
#include "stb_image.h"

#include "graphics/core/Image.hpp"


Texture::Texture() : 
    id_(0), 
    target_(0),
    width_(0), 
    height_(0)
    {}

Texture::Texture(Texture&& other) noexcept
    : id_(other.id_),
      target_(other.target_),
      width_(other.width_),
      height_(other.height_) 
{
    other.id_ = 0;
    other.target_ = 0;
    other.width_ = 0;
    other.height_ = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        // Удалить текущую текстуру, если есть
        if (id_ != 0)
            glDeleteTextures(1, &id_);

        // Перенести данные из other
        id_ = other.id_;
        target_ = other.target_;
        width_ = other.width_;
        height_ = other.height_;

        // Обнулить other
        other.id_ = 0;
        other.target_ = 0;
        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

Texture::~Texture() {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
}

void Texture::bind() const {
    glBindTexture(target_, id_);
}
void Texture::unbind() const {
    glBindTexture(target_, 0);
}

int Texture::getWidth() const {
    return width_;
}
int Texture::getHeight() const {
    return height_;
}

int Texture::getUnitId() const {
    return unit_;
}

void Texture::create(
    const Image& image, 
    TextureParams params
) {

    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
    GLuint id;
    glGenTextures(1, &id);

    glBindTexture(params.target, id);

    glTexParameteri(params.target, GL_TEXTURE_WRAP_S, params.wrap_s);
    glTexParameteri(params.target, GL_TEXTURE_WRAP_T, params.wrap_t);

    if (params.target == GL_TEXTURE_3D) {
        glTexParameteri(params.target, GL_TEXTURE_WRAP_R, params.wrap_r);
    }
    
    if (params.wrap_s == GL_CLAMP_TO_BORDER || 
        params.wrap_t == GL_CLAMP_TO_BORDER || 
        params.wrap_r == GL_CLAMP_TO_BORDER) {
        glTexParameterfv(params.target, GL_TEXTURE_BORDER_COLOR, params.border_color);
    }

    glTexParameteri(params.target, GL_TEXTURE_MAG_FILTER, params.mag_filter);
    glTexParameteri(params.target, GL_TEXTURE_MIN_FILTER, params.min_filter);
    

    switch (params.target) 
    {
        default: glTexImage2D(params.target, 0, params.internal_format, image.getWidth(), image.getHeight(), 0, image.getFormat(), GL_UNSIGNED_BYTE, image.getData()); break;
    }

    if (params.min_filter == GL_LINEAR_MIPMAP_LINEAR  || 
        params.min_filter == GL_LINEAR_MIPMAP_NEAREST ||
        params.min_filter == GL_NEAREST_MIPMAP_LINEAR ||
        params.min_filter == GL_NEAREST_MIPMAP_NEAREST)
    {
        glGenerateMipmap(params.target);
    }

    id_ = id; 
    width_ = image.getWidth(); 
    height_ = image.getHeight(); 
    target_ = params.target;
}

void Texture::create(
    int width,
    int height,
    GLenum format, 
    TextureParams params
) {

    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
    GLuint id;
    glGenTextures(1, &id);

    glBindTexture(params.target, id);

    glTexParameteri(params.target, GL_TEXTURE_WRAP_S, params.wrap_s);
    glTexParameteri(params.target, GL_TEXTURE_WRAP_T, params.wrap_t);

    if (params.target == GL_TEXTURE_3D) {
        glTexParameteri(params.target, GL_TEXTURE_WRAP_R, params.wrap_r);
    }
    
    if (params.wrap_s == GL_CLAMP_TO_BORDER || 
        params.wrap_t == GL_CLAMP_TO_BORDER || 
        params.wrap_r == GL_CLAMP_TO_BORDER) {
        glTexParameterfv(params.target, GL_TEXTURE_BORDER_COLOR, params.border_color);
    }

    glTexParameteri(params.target, GL_TEXTURE_MAG_FILTER, params.mag_filter);
    glTexParameteri(params.target, GL_TEXTURE_MIN_FILTER, params.min_filter);
    

    switch (params.target) 
    {
        default: glTexImage2D(params.target, 0, params.internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL); break;
    }

    if (params.min_filter == GL_LINEAR_MIPMAP_LINEAR  || 
        params.min_filter == GL_LINEAR_MIPMAP_NEAREST ||
        params.min_filter == GL_NEAREST_MIPMAP_LINEAR ||
        params.min_filter == GL_NEAREST_MIPMAP_NEAREST)
    {
        glGenerateMipmap(params.target);
    }

    id_ = id; 
    width_ = width; 
    height_ = height; 
    target_ = params.target;
}

void Texture::activeUnit(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    unit_ = index;
}