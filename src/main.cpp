#include <iostream>

#include "config.h"
#include "engine/Engine.hpp"
#include "scenes/MainScene.hpp"
#include "graphics/core/VertexStructures.hpp"
#include "window/Window.hpp"

class FrameBuffer {
    unsigned int framebuffer_, renderbuffer_;
    unsigned int textureColorbuffer_;

    Mesh quad;
    std::vector<ScreenVertex> quad_vertices;

    int slot_;

    public:
        FrameBuffer() : framebuffer_(0), textureColorbuffer_(0) {
            quad_vertices = {
                {{-1.0f,  1.0f}, {0.0f, 1.0f}},
                {{-1.0f, -1.0f}, {0.0f, 0.0f}},
                {{1.0f, -1.0f},  {1.0f, 0.0f}},
                {{-1.0f,  1.0f}, {0.0f, 1.0f}},
                {{1.0f, -1.0f},  {1.0f, 0.0f}},
                {{1.0f,  1.0f},  {1.0f, 1.0f}}
            };
        }
        ~FrameBuffer() {
            glDeleteFramebuffers(1, &framebuffer_);
            glDeleteTextures(1, &textureColorbuffer_);
            glDeleteRenderbuffers(1, &renderbuffer_);
        }
        void create(int width, int height) {
            quad.create<ScreenVertex>(MeshData<ScreenVertex>{quad_vertices});
            
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
        }
        void resize(int width, int height) {
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer_);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            
            glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
            glBindFramebuffer(GL_FRAMEBUFFER, 0);            
        }

        void bind() const {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glEnable(GL_DEPTH_TEST);            
        }

        void setUnitSlot(int slot = 0) {
            slot_ = slot;
            glActiveTexture(GL_TEXTURE0 + slot_);
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer_);
        }

        void drawScreen(const Shader& shader) const {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
            glClear(GL_COLOR_BUFFER_BIT);  
            
            shader.use();
            quad.draw();
        }

        int getSlot() const {return slot_;}
};


int main(void) {

    std::cout << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << std::endl;
    
    int width = 1280;
    int height = 720;

    Window::initGLFW();

{
    Window fboWindow = Window(width, height, "FBOWindow");

    auto screen_shader = Shader::create("res/shaders/framebuffer.vert", "res/shaders/framebuffer.frag");

    core::Time time;
      
    Engine engine1;
    u_ptr<MainScene> main = makeU<MainScene>(engine1);  

    FrameBuffer fbo;
    fbo.create(width, height);

    main->init(fboWindow); 

    fbo.setUnitSlot();
    screen_shader->use();
    screen_shader->uniform1i("screen_texture", fbo.getSlot());


    while (!fboWindow.shouldClose())
    {
        time.update();
        
        glfwPollEvents();

        fbo.bind();

        if (fboWindow.isResized()) {
            std::cout << "RESISED" << std::endl;
            fbo.resize(fboWindow.getWidth(), fboWindow.getHeight());
        }

        glClearColor(0.42, 0.42, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        main->update(time.getDeltaTime());

         fbo.drawScreen(*screen_shader);

        fboWindow.swapBuffers();
        
        fboWindow.eventsUpdate();
    }
}

    {
        Engine engine;

        auto wind1 = std::make_unique<Window>(width, height, "Window1");
        
        engine.addScene<MainScene>("main");

        engine.addWindow("window1", std::move(wind1));

        engine.attachSceneToWindow("main", "window1");

        engine.run();
    }
    
    Window::terminateGLFW();
    return 0;
}