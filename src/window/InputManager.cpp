#include "window/InputManager.hpp"


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
    scrollYdelta_(0.0f)
    {}



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
