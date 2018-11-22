#ifndef INPUT_H
#define INPUT_H

#include "Window.h"

class Input{
    static bool isKeyPressed[1024];

public:
    static void init();

private:
    static void onKeyClicked(GLFWwindow* , int key, int , int action, int);
};
#endif