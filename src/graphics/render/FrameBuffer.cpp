#include "graphics/render/FrameBuffer.hpp"
#include "graphics/core/Shader.hpp"
#include "core/Logger.hpp"

#include <array>
#include <glad/glad.h>

#ifdef DEBUG_MODE
    int frameBuffersCreated = 0;
#endif

FrameBuffer::FrameBuffer() : fbo_id_(0)
{}

FrameBuffer::~FrameBuffer()
{
    clear();
}

bool FrameBuffer::create(TextureParams textureParams, 
                         int width,
                         int height,
                         GLenum fbo_type,
                         GLenum attachment)
{
    if (fbo_id_ != 0) {
        clear();
    }

    // FRAMEBUFFER GENERATION
    glGenFramebuffers(1, &fbo_id_);
    glBindFramebuffer(fbo_type, fbo_id_);

    // CREATING TEXTURE
    texture_.create(width, height, textureParams);
    //
    texture_.bind();
    switch (textureParams.target)
    {
        case GL_TEXTURE_2D:
            glFramebufferTexture2D(fbo_type, attachment, textureParams.target,
                texture_.getTextureId(), 0);
        break;
        case GL_TEXTURE_CUBE_MAP:
            for (size_t i = 0; i < 6; i++)
            {
                GLenum face =  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
                glFramebufferTexture2D(fbo_type, attachment, face,
                    texture_.getTextureId(), 0);
            }
             glFramebufferTexture(fbo_type, attachment, texture_.getTextureId(), 0);
        break;
    }
    if (attachment == GL_DEPTH_ATTACHMENT)
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    // CREATING MESH
    std::array<ScreenVertex, 4> vertices = {{
    //     position       uv_coord
        {{-1.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 1.0f, -1.0f}, {1.0f, 0.0f}}, 
        {{ 1.0f,  1.0f}, {1.0f, 1.0f}},
        {{-1.0f,  1.0f}, {0.0f, 1.0f}}
    }};
    std::array<uint, 6> indices = {
        0, 1, 2,
        2, 3, 0
    }; 
    mesh_.create(vertices.size(), indices.size());
    mesh_.bind();
    mesh_.setBuffer(GL_ARRAY_BUFFER, vertices.size() * sizeof(ScreenVertex), vertices.data(), GL_DYNAMIC_DRAW);
    mesh_.setBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_DYNAMIC_DRAW);

    mesh_.setAttrib(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex),
                reinterpret_cast<void*>(offsetof(ScreenVertex, position)));

    mesh_.setAttrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex),
                reinterpret_cast<void*>(offsetof(ScreenVertex, uv_coord)));
    mesh_.unbind();

    // CHECKING FRAMEBUFFER STATUS
    if(glCheckFramebufferStatus(fbo_type) != GL_FRAMEBUFFER_COMPLETE)
    {
        core::logger.log(core::Logger::INFO, "FrameBuffer is not created");
        return false;
    }


    #ifdef DEBUG_MODE
        frameBuffersCreated++;
    #endif

    fbo_type_ = fbo_type;
    width_ = width;
    height_ = height;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void FrameBuffer::clear()
{
    if (fbo_id_ != 0)
    {
        glDeleteFramebuffers(1, &fbo_id_);
        fbo_id_ = 0;
    }
}

void FrameBuffer::bind()
{
    glBindFramebuffer(fbo_type_, fbo_id_);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(fbo_type_, 0);
}
