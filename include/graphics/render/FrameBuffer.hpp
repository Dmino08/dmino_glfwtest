#pragma once

#include "graphics/core/VertexStructures.hpp"
#include "graphics/core/Mesh.hpp"


class FrameBuffer 
{
public:
    FrameBuffer();
    ~FrameBuffer();

    bool create(GLenum texture_type, 
                int width, 
                int height, 
                GLenum fbo_type,
                GLenum attachment);
    
    void clear();

    void bind();

private:
    GLuint fbo_id_;
    Mesh mesh_;
    GLuint texture_id_;

    GLenum fbo_type_;
    int width_, height_;
};