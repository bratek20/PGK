#ifndef INPUT_H
#define INPUT_H

#include "Window.h"
#include "glm/glm.hpp"

class Input{
    static bool isKeyPressed[1024];
    static glm::vec2 mouseOffset;
    
public:
    static const float MOUSE_SENSIVITY;

    static void init();
    static void handle();

    static bool isPressed(int key);
    static glm::vec2 getMouseOffset();

private:
    static void onKeyClicked(GLFWwindow*, int key, int , int action, int);
    static void onMouseMoved(GLFWwindow*, double xpos, double ypos);
    static void onMouseClicked(GLFWwindow*, int button, int action, int mods);
};
#endif