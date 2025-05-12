#include "window/Window.hpp"
#include "core/Logger.hpp"



Window::Window(unsigned int width, unsigned int height, std::string title) : 
    _width(width),
    _height(height),
    _title(title),

    _valid(true),
    _input()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    _window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if(!_window) {
        glfwTerminate();
        logger.log(Logger::ERROR, "Window is not created");
        _valid = false;
    } 
    else {
        glfwMakeContextCurrent(_window);



        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(_window);
            glfwTerminate();
            logger.log(Logger::ERROR, "GLAD is not initialized");
            _valid = false;
        }

        _input.setWindow(_window);
    }
}



Window::~Window() {
    glfwDestroyWindow(_window);
    _window = nullptr;
}



bool Window::shouldClose() const {
    return glfwWindowShouldClose(_window);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(_window);
}

void Window::pollEvents() {
    _input.update();
}

void Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(_window, title.c_str());
}

void Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(_window, callback);
}

void Window::setCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(_window, callback);
}

void Window::setScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(_window, callback);
}

void Window::setCursorMode(int mode) {
    glfwSetInputMode(_window, GLFW_CURSOR, mode);
}


bool Window::isValid() const {
    return _valid;
}

InputManager& Window::getInput() {
    return _input;
}