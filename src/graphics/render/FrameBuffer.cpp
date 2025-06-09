#include "graphics/render/FrameBuffer.hpp"
#include "graphics/core/Shader.hpp"
#include "core/Logger.hpp"

#include <array>
#include <glad/glad.h>

#ifdef DEBUG_MODE
    int frameBuffersCreated = 0;
#endif

FrameBuffer::FrameBuffer() : fbo_id_(0), texture_id_(0)
{}

FrameBuffer::~FrameBuffer()
{
    clear();
}

bool FrameBuffer::create(GLenum texture_type, 
                         int width, 
                         int height, 
                         GLenum fbo_type, 
                         GLenum attachment)
{
    // FRAMEBUFFER GENERATION
    glGenFramebuffers(1, &fbo_id_);
    glBindFramebuffer(fbo_type, fbo_id_);

    // CREATING TEXTURE
    glGenTextures(1, &texture_id_);
    glBindTexture(texture_type, texture_id_);
    if (texture_type == GL_TEXTURE_2D)
    {
        glTexImage2D(texture_type, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, NULL); 
    }
    else if (texture_type == GL_TEXTURE_CUBE_MAP)
    {
        for(unsigned int i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
            height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        }
    }
    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //
    glFramebufferTexture2D(fbo_type, attachment, texture_type,
        texture_id_, 0);

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

    return true;
}

void FrameBuffer::clear()
{
    if (fbo_id_ != 0)
    {
        glDeleteFramebuffers(1, &fbo_id_);
        fbo_id_ = 0;
    }
    
    if (texture_id_ != 0)
    {    
        glDeleteTextures(1, &texture_id_);
        texture_id_ = 0;
    }
    mesh_.clear();
}

void FrameBuffer::bind()
{
    glBindFramebuffer(fbo_type_, fbo_id_);
}
