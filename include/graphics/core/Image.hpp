#pragma once

#include <string>
#include <glad/glad.h>

constexpr unsigned char error_data[] = {
    255, 0, 255,    0,   0, 0,    255, 0, 255,  0,   0, 0,
    0,   0, 0,      255, 0, 255,  0,   0, 0,    255, 0, 255,
    255, 0, 255,    0,   0, 0,    255, 0, 255,  0,   0, 0,
    0,   0, 0,      255, 0, 255,  0,   0, 0,    255, 0, 255
};
constexpr size_t error_size = 48;

class Image {
    unsigned char* data_;
    int width_, height_, nr_channels_;
    GLenum format_;

    public:
        Image();
        ~Image();
        int load(const std::string& path);

        const unsigned char* getData() const;
        int getWidth() const;
        int getHeight() const;
        int GetChannels() const;
        GLenum getFormat() const;

        static void flipLoad(bool value);


};