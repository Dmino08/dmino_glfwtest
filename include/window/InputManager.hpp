#pragma once


#include <GLFW/glfw3.h>
#include "window/InputKeys.hpp"

#define KEYS 1032
#define BUTTONS 8

class InputManager {
    friend class Window;

    bool keys_[KEYS];
    bool buttons_[BUTTONS];
    unsigned long frames_[KEYS + BUTTONS];

    unsigned long current_frame_;

    bool cursor_started_;
    bool cursor_locked_;

    float x_pos_;
    float y_pos_;
    float delta_x_;
    float delta_y_;

    float scroll_x_offset_;
    float scroll_y_offset_;
    float scroll_x_delta_;
    float scroll_y_delta_;

    public:
        InputManager();

        void update();

        bool pressed(int key);
        bool justPressed(int key);
        bool justReleased(int key);

        bool butPressed(int button);
        bool butJustPressed(int button);
        bool butJustReleased(int button);

        float getCursorX() const;
        float getCursorY() const;

        float getDeltaX() const;
        float getDeltaY() const;

        float getScrollX() const;
        float getScrollY() const;

        float getScrollDeltaX() const;
        float getScrollDeltaY() const;       

        bool isCursorLocked() const;
    
};