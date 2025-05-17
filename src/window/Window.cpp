#include "window/Window.hpp"
#include "core/Logger.hpp"



Window::Window(unsigned int width, unsigned int height, std::string title) : 
    width_(width),
    height_(height),
    title_(title),

    valid_(true),
    input_()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if(!window_) {
        glfwTerminate();
        logger.log(Logger::ERROR, "Window is not created");
        valid_ = false;
    } 
    else {
        glfwMakeContextCurrent(window_);



        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(window_);
            glfwTerminate();
            logger.log(Logger::ERROR, "GLAD is not initialized");
            valid_ = false;
        }

        input_.setWindow(window_);
    }
}



Window::~Window() {
    glfwDestroyWindow(window_);
    window_ = nullptr;
}



bool Window::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window_);
}

void Window::pollEvents() {
    input_.update();
}

void Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(window_, title.c_str());
}

void Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(window_, callback);
}

void Window::setCursorPosCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(window_, callback);
}

void Window::setScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(window_, callback);
}

void Window::setCursorMode(int mode) {
    glfwSetInputMode(window_, GLFW_CURSOR, mode);
}


bool Window::isValid() const {
    return valid_;
}

InputManager& Window::getInput() {
    return input_;
}