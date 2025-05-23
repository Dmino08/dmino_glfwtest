#include "window/Window.hpp"
#include "core/Logger.hpp"

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        ptr->input_.keys_[key] = true;
        ptr->input_.frames_[key] = ptr->input_.currentFrame_;
    }
    else if(action == GLFW_RELEASE) {
        ptr->input_.keys_[key] = false;
        ptr->input_.frames_[key] = ptr->input_.currentFrame_;        
    }
 
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    
    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        ptr->input_.buttons_[button] = true;
        ptr->input_.frames_[KEYS + button] = ptr->input_.currentFrame_;
    }
    else if(action == GLFW_RELEASE) {
        ptr->input_.buttons_[button] = false;
        ptr->input_.frames_[KEYS + button] = ptr->input_.currentFrame_;        
    }

}


void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    
    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(ptr->input_.cursorStarted_) {
        ptr->input_.xPos_ = xpos;
        ptr->input_.yPos_ = ypos;
        ptr->input_.deltaX_ = 0.0;
        ptr->input_.deltaY_ = 0.0;
        ptr->input_.cursorStarted_ = false;
        return;    
    }

    ptr->input_.deltaX_ += xpos - ptr->input_.xPos_;
    ptr->input_.deltaY_ += ypos - ptr->input_.yPos_;

    ptr->input_.xPos_ = xpos;
    ptr->input_.yPos_ = ypos;
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));


    ptr->input_.scrollXdelta_ += xoffset - ptr->input_.scrollXoffset_;
    ptr->input_.scrollYdelta_ += xoffset - ptr->input_.scrollYoffset_;

    ptr->input_.scrollXoffset_ = xoffset;
    ptr->input_.scrollYoffset_ = yoffset;
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    
    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    ptr->width_ = width;
    ptr->height_ = height;
    glViewport(0, 0, width, height);
}


Window::Window(int width, int height, std::string title) : 
    width_(width),
    height_(height),
    title_(title),

    valid_(true),
    resized_(false),
    input_()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    handle_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if(!handle_) {
        glfwTerminate();
        logger.log(Logger::ERROR, "Window is not created");
        valid_ = false;
    } 
    else {
        glfwMakeContextCurrent(handle_);



        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(handle_);
            glfwTerminate();
            logger.log(Logger::ERROR, "GLAD is not initialized");
            valid_ = false;
        }

        glfwSetWindowUserPointer(handle_, this);

        glfwSetKeyCallback(handle_, keyCallback);
        glfwSetMouseButtonCallback(handle_, mouseButtonCallback);
        glfwSetCursorPosCallback(handle_, cursorPosCallback);
        glfwSetScrollCallback(handle_, scrollCallback);
        glfwSetFramebufferSizeCallback(handle_, frameBufferSizeCallback);

        
    }
}



Window::~Window() {
    glfwDestroyWindow(handle_);
    handle_ = nullptr;
}



bool Window::shouldClose() const {
    return glfwWindowShouldClose(handle_);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(handle_);
}

void Window::pollEvents() {
    input_.update();
}

void Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(handle_, title.c_str());
}


void Window::setCursorMode(int mode) {
    glfwSetInputMode(handle_, GLFW_CURSOR, mode);
}

int Window::getWidth() const {
    return width_;
}
int Window::getHeight() const {
    return height_;
}


bool Window::isValid() const {
    return valid_;
}

bool Window::isResized() const {
    return resized_;
}


void Window::toggleCursor() {
    input_.cursorLocked_ = !input_.cursorLocked_;
    glfwSetInputMode(handle_, GLFW_CURSOR, input_.cursorLocked_ ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}


InputManager& Window::getInput() {
    return input_;
}

