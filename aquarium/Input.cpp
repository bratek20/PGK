#include "Input.h"
#include "Window.h"

bool Input::isKeyPressed[1024];

void Input::init(){
    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(Window::getPtr(), GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(Window::getPtr(), onKeyClicked);
}

void Input::onKeyClicked(GLFWwindow* , int key, int , int action, int)
{
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            isKeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isKeyPressed[key] = false;
        }
    }
}