#pragma once


#include <glad/glad.h>

#include <iostream>

class Image;

struct TextureParams {
    GLenum target = GL_TEXTURE_2D;

    GLenum wrap_s = GL_REPEAT;
    GLenum wrap_t = GL_REPEAT;
    GLenum wrap_r = GL_REPEAT;

    GLenum mag_filter = GL_NEAREST;
    GLenum min_filter = GL_LINEAR;

    float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    GLuint internal_format = GL_RGB;
};


class Texture {
    GLuint id_;
    GLenum target_;
    int width_, height_;

    public:
        std::string type;
        Texture(GLuint id, int width, int height, GLenum target);
        ~Texture();

        void bind() const;
        void unbind() const;

        int getWidth() const;
        int getHeight() const;

        static Texture create(
            const Image& image,
            TextureParams params = TextureParams()
        );
};
