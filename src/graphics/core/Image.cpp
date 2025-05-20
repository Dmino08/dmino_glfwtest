#include "graphics/core/Image.hpp"
#include <stb_image.h>

#include "core/Logger.hpp"

Image::~Image() {
    stbi_image_free(data_);
    data_ = nullptr;
}

int Image::load(const std::string& path) {
    
    if (data_) {
        stbi_image_free(data_);
        data_ = nullptr;
    }    
    
    data_ = stbi_load(path.c_str(), &width_, &height_, &nrChannels_, 0);

    if (!data_) {
        std::string log = "Image at the path: [" + path + "] wasn't loaded";
        logger.log(Logger::WARNING, log);
        return 1;
    }
    if (nrChannels_ == 1) format_ = GL_RED;
    else if (nrChannels_ == 3) format_ = GL_RGB;
    else if (nrChannels_ == 4) format_ = GL_RGBA;
    else {
        logger.log(Logger::WARNING, "Unknown channel count: " + std::to_string(nrChannels_));
        return 2;
    }

    return 0;
}

const unsigned char* Image::getData() const {
    return data_;
}

int Image::getWidth() const {
    return width_;
}
int Image::getHeight() const {
    return height_;
}
int Image::GetChannels() const {
    return nrChannels_;
}

GLenum Image::getFormat() const {
    return format_;
}