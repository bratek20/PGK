#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window{
    static GLFWwindow* window;
public:
    static bool init();
    static void clear();

    static GLFWwindow* getPtr();
};
#endif