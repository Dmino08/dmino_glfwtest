#include "window/InputManager.hpp"


InputManager::InputManager() : 
    keys_{}, 
    buttons_{}, 
    frames_{}, 

    current_frame_(1),
    
    cursor_started_(true),
    cursor_locked_(false),

    x_pos_(0.0f), 
    y_pos_(0.0f),
    delta_x_(0.0f),
    delta_y_(0.0f),

    scroll_x_offset_(0.0f),
    scroll_y_offset_(0.0f),
    scroll_x_delta_(0.0f),
    scroll_y_delta_(0.0f)
    {}



void InputManager::update() {

    current_frame_++;
    delta_x_ = 0.0f;
    delta_y_ = 0.0f;
    scroll_x_delta_ = 0.0f;
    scroll_y_delta_ = 0.0f;
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

    return keys_[key] && current_frame_ == frames_[key];
}

bool InputManager::justReleased(int key) {
    
    if (key < 0 || key >= KEYS) {
        return false;
    }

    return !keys_[key] && current_frame_ == frames_[key];
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
     return buttons_[button] && current_frame_ == frames_[button + KEYS];
}

bool InputManager::butJustReleased(int button) {
    
    if (button < 0 || button >= BUTTONS) {
        return false;
    }
     return !buttons_[button] && current_frame_ == frames_[button + KEYS];    
}


float InputManager::getCursorX() const {
    return x_pos_;
}

float InputManager::getCursorY() const {
    return y_pos_;
}


float InputManager::getDeltaX() const {
    return delta_x_;
}

float InputManager::getDeltaY() const {
    return delta_y_;
}

float InputManager::getScrollX() const {
    return scroll_x_offset_;
}


float InputManager::getScrollY() const {
    return scroll_y_offset_;
}

float InputManager::getScrollDeltaX() const {
    return scroll_x_delta_;
}

float InputManager::getScrollDeltaY() const {
    return scroll_y_delta_;
}


bool InputManager::isCursorLocked() const {
    return cursor_locked_;
}
