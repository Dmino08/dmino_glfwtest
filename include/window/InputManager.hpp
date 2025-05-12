#pragma once


#include <GLFW/glfw3.h>

#define KEYS 1032
#define BUTTONS 8

class InputManager {
    bool _keys[KEYS];
    bool _buttons[BUTTONS];
    unsigned long _frames[KEYS + BUTTONS];

    unsigned long _current_frame;


    bool _cursor_started;
    bool _cursor_locked;

    float _x_pos;
    float _y_pos;
    float _delta_x;
    float _delta_y;

    float _scroll_x_offset;
    float _scroll_y_offset;
    float _scroll_x_delta;
    float _scroll_y_delta;


    
    
    GLFWwindow* _window;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
     
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    public:
        InputManager();
        ~InputManager();

        void setWindow(GLFWwindow *window);

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
    
        void toggleCursor();
};