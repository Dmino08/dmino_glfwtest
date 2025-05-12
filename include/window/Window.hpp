#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window/InputManager.hpp"

class Window {
    GLFWwindow* _window;
    InputManager _input;

    unsigned int _width;
    unsigned int _height;
    std::string _title;

    bool _valid;

    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

    public:
        Window(unsigned int width, unsigned int height, std::string title);
        ~Window();

        bool shouldClose() const;
        void swapBuffers() const;

        void pollEvents();

        void setTitle(const std::string& title);
        
        void setFramebufferSizeCallback(GLFWframebuffersizefun callback);
        void setCursorPosCallback(GLFWcursorposfun callback);
        void setScrollCallback(GLFWscrollfun callback);

        void setCursorMode(int mode);

        bool isValid() const;

        InputManager& getInput();
};