#pragma once

#include "graphics/core/Image.hpp"

#include <glad/glad.h>
#include <iostream>


#define TEXTURE_LOGGING

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
    GLuint format = GL_RGB;
};


class Texture {
public:
    Texture();
    ~Texture();
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    
    void clear();
    
    GLuint getTextureUnit(); 
    GLenum getTarget();

    void bind();
    void unbind();

    int getWidth() const;
    int getHeight() const;
    int getUnitId() const;

    void generateTexture(const TextureParams& params);
    void texImage(GLenum target, int width, int height, const void *pixels, const TextureParams& params);

    void create(
        const Image& image,
        TextureParams params = TextureParams()
    );

    void create(
        int width,
        int height,
        TextureParams params
    );

    static void activeUnit(int unit);
    static int getFreeUnit();
    static int getCurrentBindedTexture(GLenum target);

private:
    GLuint id_;
    GLenum target_;
    int width_, height_;

    int unit_;

    static bool* active_units_;
    static int active_units_size_;
    static int count_;

    static void initUnits();
};
