#include "graphics/render/FrameBuffer.hpp"
#include "graphics/core/Shader.hpp"
#include "core/Logger.hpp"

#include <glad/glad.h>

#ifdef DEBUG_MODE
    int frameBuffersCreated = 0;
#endif

FrameBuffer::FrameBuffer() : framebuffer_(0), renderbuffer_(0), texture_color_buffer_(0), mesh_() {}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &framebuffer_);
    glDeleteTextures(1, &texture_color_buffer_);
    glDeleteRenderbuffers(1, &renderbuffer_);
}
void FrameBuffer::create(int width, int height) {
    if (vertices_.empty())
    {
        vertices_ = {
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f}, {0.0f, 0.0f}},
            {{1.0f, -1.0f},  {1.0f, 0.0f}},
            {{-1.0f,  1.0f}, {0.0f, 1.0f}},
            {{1.0f, -1.0f},  {1.0f, 0.0f}},
            {{1.0f,  1.0f},  {1.0f, 1.0f}}
        };
    }    
    mesh_.create(vertices_.size(), 0);
    mesh_.bind();
    mesh_.setBuffer(GL_ARRAY_BUFFER, vertices_.size() * sizeof(ScreenVertex), vertices_.data(), GL_STATIC_DRAW);
    mesh_.setAttrib(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), reinterpret_cast<void*>(offsetof(ScreenVertex, position)));
    mesh_.setAttrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), reinterpret_cast<void*>(offsetof(ScreenVertex, uv_coord)));
    mesh_.unbind();
    
    if (framebuffer_ != 0)
    {
        glDeleteFramebuffers(1, &framebuffer_);
        framebuffer_ = 0;
    }
    
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    if (texture_color_buffer_ == 0) {
        glGenTextures(1, &texture_color_buffer_);
        glBindTexture(GL_TEXTURE_2D, texture_color_buffer_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer_, 0);
    } 
    else {
        glBindTexture(GL_TEXTURE_2D, texture_color_buffer_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }

    if (renderbuffer_ != 0)
    {
        glDeleteRenderbuffers(1, &renderbuffer_);
        renderbuffer_ = 0;
    }

    glGenRenderbuffers(1, &renderbuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    

    #ifdef DEBUG_MODE
        frameBuffersCreated++;
        core::logger.log(core::Logger::INFO, "FrameBuffer " + std::to_string(frameBuffersCreated) + " is created");
    #endif
}
void FrameBuffer::resize(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, texture_color_buffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);            
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glEnable(GL_DEPTH_TEST);            
}

void FrameBuffer::setUnitSlot(int slot) {
    unit_ = slot;
    glActiveTexture(GL_TEXTURE0 + unit_);
    glBindTexture(GL_TEXTURE_2D, texture_color_buffer_);
}

void FrameBuffer::drawScreen(const Shader& shader, bool clear) const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    if (clear) {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    shader.use();
    mesh_.draw();
}

void FrameBuffer::setVertices(std::vector<ScreenVertex>&& vertices) {
    vertices_ = std::move(vertices);
}

void FrameBuffer::setMesh(Mesh&& mesh) {
    mesh_ = std::move(mesh);
}

int FrameBuffer::getUnitSlot() const {return unit_;}