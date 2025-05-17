#include "window/InputManager.hpp"


void InputManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        input->keys_[key] = true;
        input->frames_[key] = input->currentFrame_;
    }
    else if(action == GLFW_RELEASE) {
        input->keys_[key] = false;
        input->frames_[key] = input->currentFrame_;        
    }
 
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        input->buttons_[button] = true;
        input->frames_[KEYS + button] = input->currentFrame_;
    }
    else if(action == GLFW_RELEASE) {
        input->buttons_[button] = false;
        input->frames_[KEYS + button] = input->currentFrame_;        
    }

}


void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    if(input->cursorStarted_) {
        input->xPos_ = xpos;
        input->yPos_ = ypos;
        input->deltaX_ = 0.0;
        input->deltaY_ = 0.0;
        input->cursorStarted_ = false;
        return;    
    }

    input->deltaX_ += xpos - input->xPos_;
    input->deltaY_ += ypos - input->yPos_;

    input->xPos_ = xpos;
    input->yPos_ = ypos;
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));


    input->scrollXdelta_ += xoffset - input->scrollXoffset_;
    input->scrollYdelta_ += xoffset - input->scrollYoffset_;

    input->scrollXoffset_ = xoffset;
    input->scrollYoffset_ = yoffset;
}


InputManager::InputManager() : 
    keys_{}, 
    buttons_{}, 
    frames_{}, 

    currentFrame_(1),
    
    cursorStarted_(true),
    cursorLocked_(false),

    xPos_(0.0f), 
    yPos_(0.0f),
    deltaX_(0.0f),
    deltaY_(0.0f),

    scrollXoffset_(0.0f),
    scrollYoffset_(0.0f),
    scrollXdelta_(0.0f),
    scrollYdelta_(0.0f),

    window_(nullptr) 
    {}


InputManager::~InputManager() {
    window_ = nullptr;
}

void InputManager::setWindow(GLFWwindow* window)
{
    if (window_) {
        glfwSetKeyCallback(window_, nullptr);
        glfwSetMouseButtonCallback(window_, nullptr);
        glfwSetCursorPosCallback(window_, nullptr);
        glfwSetScrollCallback(window_, nullptr);
        glfwSetWindowUserPointer(window_, nullptr);
    }

    if (window) {
        window_ = window;
        glfwSetKeyCallback(window_, keyCallback);
        glfwSetMouseButtonCallback(window_, mouseButtonCallback);
        glfwSetCursorPosCallback(window_, cursorPosCallback);
        glfwSetScrollCallback(window_, scrollCallback);
        glfwSetWindowUserPointer(window_, this);
    }

}


void InputManager::update() {

    currentFrame_++;
    deltaX_ = 0.0f;
    deltaY_ = 0.0f;
    scrollXdelta_ = 0.0f;
    scrollYdelta_ = 0.0f;

    glfwPollEvents();
}

bool InputManager::pressed(int key) {
    
    if (key < 0 || key >= KEYS) {
        return false;
    }

    return keys_[key];
}

bool InputManager::justPressed(int key) {
    
    if (key < 0 || key >= KEYS) {
        return false;
    }

    return keys_[key] && currentFrame_ == frames_[key];
}

bool InputManager::justReleased(int key) {
    
    if (key < 0 || key >= KEYS) {
        return false;
    }

    return !keys_[key] && currentFrame_ == frames_[key];
}


bool InputManager::butPressed(int button) {

    if (button < 0 || button >= BUTTONS) {
        return false;
    }
     return buttons_[button];
}

bool InputManager::butJustPressed(int button) {
    
    if (button < 0 || button >= BUTTONS) {
        return false;
    }
     return buttons_[button] && currentFrame_ == frames_[button + KEYS];
}

bool InputManager::butJustReleased(int button) {
    
    if (button < 0 || button >= BUTTONS) {
        return false;
    }
     return !buttons_[button] && currentFrame_ == frames_[button + KEYS];    
}


float InputManager::getCursorX() const {
    return xPos_;
}

float InputManager::getCursorY() const {
    return yPos_;
}


float InputManager::getDeltaX() const {
    return deltaX_;
}

float InputManager::getDeltaY() const {
    return deltaY_;
}

float InputManager::getScrollX() const {
    return scrollXoffset_;
}


float InputManager::getScrollY() const {
    return scrollYoffset_;
}

float InputManager::getScrollDeltaX() const {
    return scrollXdelta_;
}

float InputManager::getScrollDeltaY() const {
    return scrollYdelta_;
}


bool InputManager::isCursorLocked() const {
    return cursorLocked_;
}


void InputManager::toggleCursor() {
    cursorLocked_ = !cursorLocked_;
    glfwSetInputMode(window_, GLFW_CURSOR, cursorLocked_ ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}