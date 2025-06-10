#pragma once

#include "graphics/core/Image.hpp"

#include <glad/glad.h>
#include <iostream>


struct TextureParams {
    GLenum target = GL_TEXTURE_2D;

    GLenum wrap_s = GL_REPEAT;
    GLenum wrap_t = GL_REPEAT;
    GLenum wrap_r = GL_REPEAT;

    GLenum mag_filter = GL_NEAREST;
    GLenum min_filter = GL_LINEAR;

    GLenum type = GL_UNSIGNED_BYTE;

    float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    GLuint internal_format = GL_RGB;
};


class Texture {
    GLuint id_;
    GLenum target_;
    int width_, height_;

    int unit_;

    public:
        Texture();
        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;
        ~Texture();

        void clear();
        
        GLuint getTextureId(); 
        GLenum getTarget();

        void bind() const;
        void unbind() const;

        int getWidth() const;
        int getHeight() const;
        int getUnitId() const;

        void create(
            const Image& image,
            TextureParams params = TextureParams()
        );
        void Texture::create(
            int width,
            int height,
            GLenum format, 
            TextureParams params
        );

        void activeUnit(int index);
};
