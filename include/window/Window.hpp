#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window/InputManager.hpp"

#define TYPE_GLFW 0
using WindowType = int;

class Window {
    GLFWwindow* handle_;
    InputManager input_;

    int width_;
    int height_;
    std::string title_;

    bool valid_;
    bool resized_;

    void initGLAD();

    static bool is_glfw_initialized_;
    static bool is_glad_initialized_;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
     
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

    

    public:
        Window(int width, int height, std::string title, WindowType type = TYPE_GLFW);
        ~Window();

        bool shouldClose() const;
        void swapBuffers() const;

        void makeContextCurrent() const;
        bool isContext() const;

        void eventsUpdate();

        void setTitle(const std::string& title);
        void setCursorMode(int mode);
    
        int getWidth() const;
        int getHeight() const;

        bool isValid() const;
        bool isResized() const;

        void toggleCursor();

        InputManager& getInput();

        static int initGLFW();
        static void terminateGLFW();
        
};