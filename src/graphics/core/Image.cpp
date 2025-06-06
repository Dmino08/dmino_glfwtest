#include "graphics/core/Image.hpp"
#include <stb_image.h>

#include "core/Logger.hpp"


#ifdef DEBUG_MODE
    int imagesLoaded = 0;
#endif


Image::Image() : data_(nullptr)
{}


Image::~Image() {
    stbi_image_free(data_);
    data_ = nullptr;
}

int Image::load(const std::string& path) {
    
    int complete_code = 0;

    if (data_) {
        stbi_image_free(data_);
        data_ = nullptr;
    }    
    
    data_ = stbi_load(path.c_str(), &width_, &height_, &nr_channels_, 0);


    if (!data_) {
        std::string log = "Image at the path: [" + path + "] wasn't loaded";
        core::logger.log(core::Logger::WARNING, log);

        data_ = new unsigned char[error_size];
        std::memcpy(data_, error_data, error_size);
        width_ = 4;
        height_ = 4;
        nr_channels_ = 3;

        complete_code = 1;
    }
    if (nr_channels_ == 1) format_ = GL_RED;
    else if (nr_channels_ == 3) format_ = GL_RGB;
    else if (nr_channels_ == 4) format_ = GL_RGBA;
    else {
        core::logger.log(core::Logger::WARNING, "Unknown channel count: " + std::to_string(nr_channels_));
        complete_code = 2;
    }
    
    #ifdef DEBUG_MODE
        imagesLoaded++;
        core::logger.log(core::Logger::INFO, "Image " + std::to_string(imagesLoaded) + " is loaded");
    #endif

    return complete_code;
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
    return nr_channels_;
}

GLenum Image::getFormat() const {
    return format_;
}

void Image::flipLoad(bool value) {
    stbi_set_flip_vertically_on_load(value);
}