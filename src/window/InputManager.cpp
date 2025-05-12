#include "window/InputManager.hpp"


void InputManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        input->_keys[key] = true;
        input->_frames[key] = input->_current_frame;
    }
    else if(action == GLFW_RELEASE) {
        input->_keys[key] = false;
        input->_frames[key] = input->_current_frame;        
    }
 
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        input->_buttons[button] = true;
        input->_frames[KEYS + button] = input->_current_frame;
    }
    else if(action == GLFW_RELEASE) {
        input->_buttons[button] = false;
        input->_frames[KEYS + button] = input->_current_frame;        
    }

}


void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

    if(input->_cursor_started) {
        input->_x_pos = xpos;
        input->_y_pos = ypos;
        input->_delta_x = 0.0;
        input->_delta_y = 0.0;
        input->_cursor_started = false;
        return;    
    }

    input->_delta_x += xpos - input->_x_pos;
    input->_delta_y += ypos - input->_y_pos;

    input->_x_pos = xpos;
    input->_y_pos = ypos;
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));


    input->_scroll_x_delta += xoffset - input->_scroll_x_offset;
    input->_scroll_y_delta += xoffset - input->_scroll_y_offset;

    input->_scroll_x_offset = xoffset;
    input->_scroll_y_offset = yoffset;
}


InputManager::InputManager() : 
    _keys{}, 
    _buttons{}, 
    _frames{}, 

    _current_frame(1),
    
    _cursor_started(true),
    _cursor_locked(false),

    _x_pos(0.0f), 
    _y_pos(0.0f),
    _delta_x(0.0f),
    _delta_y(0.0f),

    _scroll_x_offset(0.0f),
    _scroll_y_offset(0.0f),
    _scroll_x_delta(0.0f),
    _scroll_y_delta(0.0f),

    _window(nullptr) 
    {}


InputManager::~InputManager() {
    _window = nullptr;
}

void InputManager::setWindow(GLFWwindow* window)
{
    if (_window) {
        glfwSetKeyCallback(_window, nullptr);
        glfwSetMouseButtonCallback(_window, nullptr);
        glfwSetCursorPosCallback(_window, nullptr);
        glfwSetScrollCallback(_window, nullptr);
        glfwSetWindowUserPointer(_window, nullptr);
    }

    if (window) {
        _window = window;
        glfwSetKeyCallback(_window, keyCallback);
        glfwSetMouseButtonCallback(_window, mouseButtonCallback);
        glfwSetCursorPosCallback(_window, cursorPosCallback);
        glfwSetScrollCallback(_window, scrollCallback);
        glfwSetWindowUserPointer(_window, this);
    }

}


void InputManager::update() {

    _current_frame++;
    _delta_x = 0.0f;
    _delta_y = 0.0f;
    _scroll_x_delta = 0.0f;
    _scroll_y_delta = 0.0f;

    glfwPollEvents();
}

bool InputManager::pressed(int key) {
    
    if (key < 0 || key >= KEYS) {
        return false;
    }

    return _keys[key];
}

bool InputManager::justPressed(int key) {
    
    if (key < 0 || key >= KEYS) {
        return false;
    }

    return _keys[key] && _current_frame == _frames[key];
}

bool InputManager::justReleased(int key) {
    
    if (key < 0 || key >= KEYS) {
        return false;
    }

    return !_keys[key] && _current_frame == _frames[key];
}


bool InputManager::butPressed(int button) {

    if (button < 0 || button >= BUTTONS) {
        return false;
    }
     return _buttons[button];
}

bool InputManager::butJustPressed(int button) {
    
    if (button < 0 || button >= BUTTONS) {
        return false;
    }
     return _buttons[button] && _current_frame == _frames[button + KEYS];
}

bool InputManager::butJustReleased(int button) {
    
    if (button < 0 || button >= BUTTONS) {
        return false;
    }
     return !_buttons[button] && _current_frame == _frames[button + KEYS];    
}


float InputManager::getCursorX() const {
    return _x_pos;
}

float InputManager::getCursorY() const {
    return _y_pos;
}


float InputManager::getDeltaX() const {
    return _delta_x;
}

float InputManager::getDeltaY() const {
    return _delta_y;
}

float InputManager::getScrollX() const {
    return _scroll_x_offset;
}


float InputManager::getScrollY() const {
    return _scroll_y_offset;
}

float InputManager::getScrollDeltaX() const {
    return _scroll_x_delta;
}

float InputManager::getScrollDeltaY() const {
    return _scroll_y_delta;
}


bool InputManager::isCursorLocked() const {
    return _cursor_locked;
}


void InputManager::toggleCursor() {
    _cursor_locked = !_cursor_locked;
    glfwSetInputMode(_window, GLFW_CURSOR, _cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}