#pragma once


#include <GLFW/glfw3.h>

#define KEYS 1032
#define BUTTONS 8

class InputManager {
    bool keys_[KEYS];
    bool buttons_[BUTTONS];
    unsigned long frames_[KEYS + BUTTONS];

    unsigned long currentFrame_;


    bool cursorStarted_;
    bool cursorLocked_;

    float xPos_;
    float yPos_;
    float deltaX_;
    float deltaY_;

    float scrollXoffset_;
    float scrollYoffset_;
    float scrollXdelta_;
    float scrollYdelta_;


    
    
    GLFWwindow* window_;

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