#include "graphics/core/Texture.hpp"

#include "stb_image.h"
#include "graphics/core/Image.hpp"
#include "core/Logger.hpp"

bool* Texture::active_units_ = nullptr;
int Texture::active_units_size_ = 0;
int Texture::count_ = 0;

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

GLuint Texture::getTextureUnit() {
    return id_;
}
GLenum Texture::getTarget() {
    return target_;
}

void Texture::bind() {
    glGetIntegerv(GL_ACTIVE_TEXTURE, &unit_);
    unit_ -= GL_TEXTURE0;
    Texture::active_units_[unit_] = true;
    glBindTexture(target_, id_);
}
void Texture::unbind() {
    unit_ = -1;
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
}

void Texture::texImage(GLenum target, int width, int height, const void* pixels, const TextureParams& params) {
    glTexImage2D(target, 0, params.internal_format, width, height, 0, params.format,
        params.type, pixels);    
}

void Texture::create(
    const Image& image, 
    TextureParams params) {

    GLint prev_texture;    
    prev_texture = Texture::getCurrentBindedTexture(params.target);

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


    glBindTexture(params.target, prev_texture);
}


void Texture::create(
    int width,
    int height,
    TextureParams params) {

    GLint prev_texture;    
    prev_texture = Texture::getCurrentBindedTexture(params.target);


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

    glBindTexture(params.target, prev_texture);
}



void Texture::activeUnit(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
}

int Texture::getFreeUnit() {
    int free_unit = -1;
    if (active_units_) {
        for (size_t i = 0; i < active_units_size_; i++) {
            if (active_units_[i] == false) {
                free_unit = i;
                break;
            }
        }
    }
    return free_unit;
}

int Texture::getCurrentBindedTexture(GLenum target) {
    GLint unit = -1;
    switch (target)
    {
        case GL_TEXTURE_2D:
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &unit);
        break;
        case GL_TEXTURE_3D:
            glGetIntegerv(GL_TEXTURE_BINDING_3D, &unit);
        break;
        case GL_TEXTURE_CUBE_MAP:
            glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &unit);
        break;
    }
    return unit;
}

void Texture::initUnits() {
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &active_units_size_);
    active_units_ = new bool[active_units_size_];
    std::fill(&active_units_[0], &active_units_[0] + active_units_size_, false);
}


