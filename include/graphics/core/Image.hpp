#pragma once

#include <string>
#include <glad/glad.h>

class Image {
    unsigned char* data_;
    int width_, height_, nrChannels_;
    GLenum format_;

    public:
        Image() = default;
        ~Image();
        int load(const std::string& path);

        const unsigned char* getData() const;
        int getWidth() const;
        int getHeight() const;
        int GetChannels() const;
        GLenum getFormat() const;


};