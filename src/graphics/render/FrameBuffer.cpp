#include "graphics/render/FrameBuffer.hpp"
#include "graphics/core/Shader.hpp"

#include <glad/glad.h>

#ifdef DEBUG_MODE
    int frameBuffersCreated = 0;
#endif

FrameBuffer::FrameBuffer() : framebuffer_(0), renderbuffer_(0), textureColorbuffer_(0) {}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &framebuffer_);
    glDeleteTextures(1, &textureColorbuffer_);
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
    mesh_.create<ScreenVertex>(MeshData<ScreenVertex>{vertices_});
    
    if (framebuffer_ != 0)
    {
        glDeleteFramebuffers(1, &framebuffer_);
        framebuffer_ = 0;
    }
    
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    if (textureColorbuffer_ == 0) {
        glGenTextures(1, &textureColorbuffer_);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer_, 0);
    } 
    else {
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer_);
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
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer_);
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
    slot_ = slot;
    glActiveTexture(GL_TEXTURE0 + slot_);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer_);
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

int FrameBuffer::getSlot() const {return slot_;}