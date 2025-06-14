#include "graphics/core/Texture.hpp"

#include "stb_image.h"
#include "graphics/core/Image.hpp"
#include "core/Logger.hpp"

bool* Texture::active_units_ = nullptr;
int Texture::active_units_size_ = 0;
int Texture::count_ = 0;

void Texture::initUnits() {
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &active_units_size_);
    active_units_ = new bool[active_units_size_];
    std::fill(&active_units_[0], &active_units_[0] + active_units_size_, false);
}

void Texture::setUnit() {
    GLint texture_unit;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &texture_unit);
    texture_unit -= GL_TEXTURE0;
    if (Texture::active_units_[texture_unit] == true) {
        unit_ = texture_unit;
        #ifdef TEXTURE_LOGGING
            core::logger.log(core::Logger::INFO, "You've used this texture slot before! Unit: " + std::to_string(texture_unit));
        #endif          
    }
    if (texture_unit >= 0 && texture_unit < active_units_size_) {
        Texture::active_units_[texture_unit] = true;
        unit_ = texture_unit;
        #ifdef TEXTURE_LOGGING
            core::logger.log(core::Logger::INFO, "Used texture unit: " + std::to_string(texture_unit));
        #endif  
    }
    else {
        unit_ = active_units_size_ - 1;
        #ifdef TEXTURE_LOGGING
            core::logger.log(core::Logger::ERROR, "Incorrect texture unit: " + std::to_string(texture_unit));
        #endif 
    }
}

Texture::Texture() : 
    id_(0), 
    target_(0),
    width_(0), 
    height_(0),
    unit_(-1) {
        if (Texture::active_units_ == nullptr) {
            initUnits();
            count_ = 0;
        }
        count_++;
        #ifdef TEXTURE_LOGGING
            core::logger.log(core::Logger::INFO, "Texture count: " + std::to_string(count_));
        #endif        
}

Texture::Texture(const Texture& other)     
    : id_(other.id_),
      target_(other.target_),
      width_(other.width_),
      height_(other.height_),
      unit_(other.unit_) {}

Texture& Texture::operator=(const Texture& other) {
    if (this != &other) {
        clear();

        id_ = other.id_;
        target_ = other.target_;
        width_ = other.width_;
        height_ = other.height_;
        unit_ = other.unit_;

        count_++;
    }
    return *this;    
}

Texture::Texture(Texture&& other) noexcept
    : id_(other.id_),
      target_(other.target_),
      width_(other.width_),
      height_(other.height_),
      unit_(other.unit_) {

    other.id_ = 0;
    other.target_ = 0;
    other.width_ = 0;
    other.height_ = 0;
    other.unit_ = -1;

    count_++;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        clear();

        id_ = other.id_;
        target_ = other.target_;
        width_ = other.width_;
        height_ = other.height_;
        unit_ = other.unit_;

        other.id_ = 0;
        other.target_ = 0;
        other.width_ = 0;
        other.height_ = 0;
        other.unit_ = -1;

        count_++;
    }
    return *this;
}

Texture::~Texture() {
    clear();
    if (--Texture::count_ == 0) {
            #ifdef TEXTURE_LOGGING
                core::logger.log(core::Logger::INFO, "Texture units are cleared");
            #endif
        if (Texture::active_units_ != nullptr) {
            delete [] Texture::active_units_;
            Texture::active_units_ = nullptr;
            Texture::active_units_size_ = 0;
        }  
    }
}

void Texture::clear() {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
        active_units_[unit_] = false;
        #ifdef TEXTURE_LOGGING
            core::logger.log(core::Logger::INFO, "Unit " + std::to_string(unit_) + " is freed");
        #endif
        unit_ = -1;
    }
}

GLuint Texture::getTextureId() {
    return id_;
}
GLenum Texture::getTarget() {
    return target_;
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

void Texture::generateTexture(const TextureParams& params) {
    glGenTextures(1, &id_);

    glBindTexture(params.target, id_);

    glTexParameteri(params.target, GL_TEXTURE_WRAP_S, params.wrap_s);
    glTexParameteri(params.target, GL_TEXTURE_WRAP_T, params.wrap_t);

    if (params.target == GL_TEXTURE_3D || params.target == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(params.target, GL_TEXTURE_WRAP_R, params.wrap_r);
    }
    
    if (params.wrap_s == GL_CLAMP_TO_BORDER || 
        params.wrap_t == GL_CLAMP_TO_BORDER || 
        params.wrap_r == GL_CLAMP_TO_BORDER) {
        glTexParameterfv(params.target, GL_TEXTURE_BORDER_COLOR, params.border_color);
    }

    glTexParameteri(params.target, GL_TEXTURE_MAG_FILTER, params.mag_filter);
    glTexParameteri(params.target, GL_TEXTURE_MIN_FILTER, params.min_filter);

    setUnit();
}

void Texture::texImage(GLenum target, int width, int height, const void* pixels, const TextureParams& params) {
    glTexImage2D(target, 0, params.internal_format, width, height, 0, params.format,
        params.type, pixels);    
}

void Texture::create(
    const Image& image, 
    TextureParams params) {

    clear();
    generateTexture(params);
    width_ = image.getWidth(); 
    height_ = image.getHeight(); 
    target_ = params.target;
    params.format = image.getFormat();
    
    switch (params.target) {
        case GL_TEXTURE_2D:
            texImage(GL_TEXTURE_2D, width_, height_, image.getData(), params);
        break;
        case GL_TEXTURE_CUBE_MAP:
            for(unsigned int i = 0; i < 6; i++) {
                texImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width_, height_, image.getData(), params);
            }           
        break;
    }

    if (params.min_filter == GL_LINEAR_MIPMAP_LINEAR  || 
        params.min_filter == GL_LINEAR_MIPMAP_NEAREST ||
        params.min_filter == GL_NEAREST_MIPMAP_LINEAR ||
        params.min_filter == GL_NEAREST_MIPMAP_NEAREST) {
        glGenerateMipmap(params.target);
    }
}


void Texture::create(
    int width,
    int height,
    TextureParams params) {

    clear();
    generateTexture(params);
    width_ = width; 
    height_ = height; 
    target_ = params.target;

    switch (params.target) {
        case GL_TEXTURE_2D:
            texImage(GL_TEXTURE_2D, width_, height_, NULL, params);
        break;
        case GL_TEXTURE_CUBE_MAP:
            for(unsigned int i = 0; i < 6; i++) {
                texImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width_, height_, NULL, params);
            }           
        break;
    }

    if (params.min_filter == GL_LINEAR_MIPMAP_LINEAR  || 
        params.min_filter == GL_LINEAR_MIPMAP_NEAREST ||
        params.min_filter == GL_NEAREST_MIPMAP_LINEAR ||
        params.min_filter == GL_NEAREST_MIPMAP_NEAREST)
    {
        glGenerateMipmap(params.target);
    }


}

void Texture::activeUnit(int index) {
    if (index >= active_units_size_ || index < 0) {
        #ifdef TEXTURE_LOGGING
            core::logger.log(core::Logger::ERROR, "Incorrect texture unit: " + std::to_string(index));
        #endif 
        return;
    }
    
    if (active_units_[index] == true) {
        #ifdef TEXTURE_LOGGING
            core::logger.log(core::Logger::WARNING, "You've used this texture slot before! Unit: " + std::to_string(index));
        #endif
    }
    
    glActiveTexture(GL_TEXTURE0 + index);
}