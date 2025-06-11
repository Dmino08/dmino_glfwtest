#pragma once

#include "graphics/core/VertexStructures.hpp"
#include "graphics/core/Mesh.hpp"
#include "graphics/core/Texture.hpp"

constexpr TextureParams DIRECTION_DEPTH_PARAMS = 
{
    GL_TEXTURE_2D,

    GL_CLAMP_TO_BORDER,
    GL_CLAMP_TO_BORDER,
    GL_CLAMP_TO_BORDER,

    GL_NEAREST,
    GL_NEAREST,

    GL_FLOAT,

    {1.0f, 1.0f, 1.0f, 1.0f},

    GL_DEPTH_COMPONENT32F,
    GL_DEPTH_COMPONENT
};

constexpr TextureParams POINT_DEPTH_PARAMS = 
{
    GL_TEXTURE_CUBE_MAP,

    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_EDGE,

    GL_NEAREST,
    GL_NEAREST,

    GL_FLOAT,

    {1.0f, 1.0f, 1.0f, 1.0f},

    GL_DEPTH_COMPONENT,
    GL_DEPTH_COMPONENT
};

class FrameBuffer 
{
public:
    FrameBuffer();
    ~FrameBuffer();

    bool create(TextureParams textureParams,
                int width,
                int height, 
                GLenum fbo_type,
                GLenum attachment);
    
    void clear();

    void bind(); 
    void unbind();

private:
    GLuint fbo_id_;
    Mesh mesh_;
    Texture texture_;

    GLenum fbo_type_;
    int width_, height_;
};