#include "Camera.h"
#include "Input.h"
#include "Globals.h"

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <functional>
#include <algorithm>

using namespace std;

const glm::vec3 Camera::LOCAL_UP = glm::vec3(0,1,0);


Camera::Camera(glm::vec3 initPos) : position(initPos), zoom(0.5f) {
    const static float ZOOM_STEP = 0.1f;
    Input::onKeyPressed(GLFW_KEY_O, std::bind(&Camera::changeZoom, this, ZOOM_STEP));
    Input::onKeyPressed(GLFW_KEY_P, std::bind(&Camera::changeZoom, this, -ZOOM_STEP));
}

CameraPtr Camera::create(glm::vec3 initPos){
    return CameraPtr(new Camera(initPos));
}

void Camera::update(){
    static const float VELOCITY = 10;
    float stepVal = VELOCITY * Globals::deltaTime;
    glm::vec3 step = glm::vec3(0);

    if(Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)){
        step = {0, 0, -stepVal};
    }
    if(Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)){
        step = {0, 0, stepVal};
    }
    if(Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)){
        step = {stepVal, 0, 0};
    }
    if(Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)){
        step = {-stepVal, 0, 0};
    }
    position += step;
}

glm::vec2 Camera::getPos2D() const {
    return glm::vec2(position.x, position.z);
}

void Camera::changeZoom(float step){
    static const float MIN_ZOOM = 0.1f;
    static const float MAX_ZOOM = 3.0f;

    zoom += step;
    zoom = max(zoom, MIN_ZOOM);
    zoom = min(zoom, MAX_ZOOM);
}

float Camera::getZoom() const {
    return zoom;
}

glm::mat4 Camera::getViewMat(){
    return glm::lookAt(
                position, // the position of your camera, in world space
                position,   // where you want to look at, in world space
                LOCAL_UP       // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
            );
}

glm::mat4 Camera::getProjectionMat(){
    float fov = 45.0f;
    return glm::perspective(
                glm::radians(fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
                4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
                0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
                400.0f             // Far clipping plane. Keep as little as possible.
            );
}