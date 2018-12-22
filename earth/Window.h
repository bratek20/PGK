#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window{
    static GLFWwindow* window;
public:
    static bool open();
    static void clear();
    static void swapBuffers();
    static bool shouldClose();
    static void close();

    static GLFWwindow* getPtr();
};
#endif