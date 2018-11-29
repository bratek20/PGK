#include "Input.h"
#include "Window.h"

bool Input::isKeyPressed[1024];
glm::vec2 Input::mouseOffset;
const float Input::MOUSE_SENSIVITY = 1.0f;

void Input::init(){
    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(Window::getPtr(), GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(Window::getPtr(), onKeyClicked);
    glfwSetCursorPosCallback(Window::getPtr(), onMouseMoved);
    glfwSetMouseButtonCallback(Window::getPtr(), onMouseClicked);
}

void Input::handle(){
    glfwPollEvents();
}

bool Input::isPressed(int key){
    return isKeyPressed[key];
}

glm::vec2 Input::getMouseOffset(){
    return mouseOffset;
}

void Input::onKeyClicked(GLFWwindow*, int key, int, int action, int){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(Window::getPtr(), GL_TRUE);
    }

    if (key >= 0 && key < 1024){
        if (action == GLFW_PRESS){
            isKeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE){
            isKeyPressed[key] = false;
        }
    }
}

void Input::onMouseMoved(GLFWwindow*, double xpos, double ypos){
    static bool firstMouseMove = true;
    static double lastX = 0.0;
    static double lastY = 0.0;

    if (firstMouseMove){
        lastX = xpos;
        lastY = ypos;
        firstMouseMove = 0;
    }

    mouseOffset.x = xpos - lastX;
    mouseOffset.y = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;
	
    mouseOffset *= MOUSE_SENSIVITY;
}

void Input::onMouseClicked(GLFWwindow*, int button, int action, int){
    if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
        
    }
}