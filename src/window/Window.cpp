#include "window/Window.hpp"
#include "core/Logger.hpp"


bool Window::is_glfw_initialized_ = false;
bool Window::is_glad_initialized_ = false;


void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        ptr->input_.keys_[key] = true;
        ptr->input_.frames_[key] = ptr->input_.current_frame_;
    }
    else if(action == GLFW_RELEASE) {
        ptr->input_.keys_[key] = false;
        ptr->input_.frames_[key] = ptr->input_.current_frame_;        
    }
 
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    
    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        ptr->input_.buttons_[button] = true;
        ptr->input_.frames_[KEYS + button] = ptr->input_.current_frame_;
    }
    else if(action == GLFW_RELEASE) {
        ptr->input_.buttons_[button] = false;
        ptr->input_.frames_[KEYS + button] = ptr->input_.current_frame_;        
    }

}


void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    
    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(ptr->input_.cursor_started_) {
        ptr->input_.x_pos_ = xpos;
        ptr->input_.y_pos_ = ypos;
        ptr->input_.delta_x_ = 0.0;
        ptr->input_.delta_y_ = 0.0;
        ptr->input_.cursor_started_ = false;
        return;    
    }

    ptr->input_.delta_x_ += xpos - ptr->input_.x_pos_;
    ptr->input_.delta_y_ += ypos - ptr->input_.y_pos_;

    ptr->input_.x_pos_ = xpos;
    ptr->input_.y_pos_ = ypos;
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));


    ptr->input_.scroll_x_delta_ += xoffset - ptr->input_.scroll_x_offset_;
    ptr->input_.scroll_y_delta_ += xoffset - ptr->input_.scroll_y_offset_;

    ptr->input_.scroll_x_offset_ = xoffset;
    ptr->input_.scroll_y_offset_ = yoffset;
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    
    Window* ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    GLFWwindow* prev = glfwGetCurrentContext();

    if (ptr->handle_ != prev)
    {
        ptr->makeContextCurrent();
        ptr->resized_ = true;
        ptr->width_ = width;
        ptr->height_ = height;
        glViewport(0, 0, width, height);

        glfwMakeContextCurrent(prev);
    }
    else {
        ptr->resized_ = true;
        ptr->width_ = width;
        ptr->height_ = height;
        glViewport(0, 0, width, height);        
    }
}

void Window::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        core::logger.log(core::Logger::ERROR, "GLAD is not initialized"); 
        return;
    }
    is_glad_initialized_ = true;

    #ifdef DEBUG_MOD
        core::logger.log(core::Logger::INFO, "GLAD is initialized"); 
    #endif
}



Window::Window(int width, int height, std::string title, WindowType type) : 
    width_(width),
    height_(height),
    title_(title),

    valid_(false),
    resized_(false),
    input_()
{
    if (type == TYPE_GLFW && is_glfw_initialized_ == true)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);

        handle_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if(!handle_) {
            core::logger.log(core::Logger::ERROR, "Window is not created");
            return;
        } 
        #ifdef DEBUG_MOD
            core::logger.log(core::Logger::INFO, "GLFW Window is created"); 
        #endif

        glfwMakeContextCurrent(handle_);
        

        if (!is_glad_initialized_) {
            initGLAD();
            if (!is_glad_initialized_) {
                glfwDestroyWindow(handle_);
                handle_ = nullptr;
                return;
            }
        }

        glfwSetWindowUserPointer(handle_, this);

        glfwSetKeyCallback(handle_, keyCallback);
        glfwSetMouseButtonCallback(handle_, mouseButtonCallback);
        glfwSetCursorPosCallback(handle_, cursorPosCallback);
        glfwSetScrollCallback(handle_, scrollCallback);
        glfwSetFramebufferSizeCallback(handle_, frameBufferSizeCallback);

        valid_ = true;
        
    }
}



Window::~Window() {
    if (handle_ != nullptr) {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
    }
}



bool Window::shouldClose() const {
    return glfwWindowShouldClose(handle_);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(handle_);
}

void Window::makeContextCurrent() const {
    glfwMakeContextCurrent(handle_);
}

bool Window::isContext() const{
    return handle_ == glfwGetCurrentContext();
}

void Window::eventsUpdate() {
    input_.update();
    resized_ = false;
}

void Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(handle_, title.c_str());
}


void Window::setCursorMode(int mode) {
    glfwSetInputMode(handle_, GLFW_CURSOR, mode);
}

void Window::setCloseCallback(GLFWwindowclosefun callback) {
    glfwSetWindowCloseCallback(handle_, callback);
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
    input_.cursor_locked_ = !input_.cursor_locked_;
    glfwSetInputMode(handle_, GLFW_CURSOR, input_.cursor_locked_ ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}


InputManager& Window::getInput() {
    return input_;
}

int Window::initGLFW() {
    if(!glfwInit()) {
        core::logger.log(core::Logger::ERROR, "GLFW is not initialized");
        return -1;
    }
    is_glfw_initialized_ = true;
    core::logger.log(core::Logger::INFO, "GLFW is initialized"); 
    return 0;
}

void Window::terminateGLFW() {
    glfwTerminate();
}