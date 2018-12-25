#include "Camera.h"
#include "Input.h"
#include "Globals.h"
#include "Window.h"

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <functional>
#include <algorithm>
#include <cmath>

using namespace std;

const glm::vec3 Camera::LOCAL_UP = glm::vec3(0,1,0);


Camera::Camera(glm::vec3 initPos) : position(initPos), zoom(1.0f) {
    Input::onKeyPressed(GLFW_KEY_O, std::bind(&Camera::changeZoom, this, 1));
    Input::onKeyPressed(GLFW_KEY_P, std::bind(&Camera::changeZoom, this, -1));
}

CameraPtr Camera::create(glm::vec3 initPos){
    return CameraPtr(new Camera(initPos));
}

void Camera::update(){
    static const float VELOCITY = 1;
    float stepVal = VELOCITY * Globals::deltaTime / zoom;
    glm::vec3 step = glm::vec3(0);

    if(Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)){
        step = {0, 0, stepVal};
    }
    if(Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)){
        step = {0, 0, -stepVal};
    }
    if(Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)){
        step = {-stepVal, 0, 0};
    }
    if(Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)){
        step = {stepVal, 0, 0};
    }
    position += step;
}

glm::vec2 Camera::getPos2D() const {
    return glm::vec2(position.x, position.z);
}

void Camera::changeZoom(int dir){
    static const float MIN_ZOOM = 0.1f;
    static const float MAX_ZOOM = 10.0f;
    
    float step = MIN_ZOOM;
    if(zoom > 1){
        step = 0.5f;
    }
    else if(zoom >= 5){
        step = 1.0f;
    }

    zoom += dir * step;
    zoom = max(zoom, MIN_ZOOM);
    zoom = min(zoom, MAX_ZOOM);
    cout << "Zoom: " << zoom << endl;
}

float Camera::getZoom() const {
    return zoom;
}

std::pair<int,int> Camera::getCenterSegment(){
    auto pos = getPos2D();
    return {floor(pos.x), floor(pos.y)};
}

std::pair<int,int> Camera::getViewSize(){
    float width = 2.0f / (zoom * Window::getRatio());
    float height = 2.0f / zoom;
    return {ceil(width) + 1, ceil(height) + 1};
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