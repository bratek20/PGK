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
const float Camera::RADIUS = 100000;//6 371 000;

Camera::Camera(glm::vec2 initPos) : pos2D(initPos), zoom(1.0f), height(2500.0f), yaw(-90.0f), pitch(0.0f) {
    Input::onKeyPressed(GLFW_KEY_O, std::bind(&Camera::changeZoom, this, 1));
    Input::onKeyPressed(GLFW_KEY_P, std::bind(&Camera::changeZoom, this, -1));
    Input::onKeyPressed(GLFW_KEY_K, std::bind(&Camera::changeHeight, this, 1));
    Input::onKeyPressed(GLFW_KEY_L, std::bind(&Camera::changeHeight, this, -1));
}

CameraPtr Camera::create(glm::vec2 initPos){
    return CameraPtr(new Camera(initPos));
}

void Camera::update(bool is2D){
    if(is2D){
        update2D();
    }
    else{
        update3D();
    }   
}

void Camera::update2D(){
    static const float VELOCITY = 1;
    float stepVal = VELOCITY * Globals::deltaTime / zoom;
    glm::vec2 step = glm::vec2(0);

    if(Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)){
        step = {0, stepVal};
    }
    if(Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)){
        step = {0, -stepVal};
    }
    if(Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)){
        step = {-stepVal, 0};
    }
    if(Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)){
        step = {stepVal, 0};
    }
    pos2D += step;
}

void Camera::update3D(){
    static const float VELOCITY = 1000;
    float stepVal = VELOCITY * Globals::deltaTime;
    
    static const float MOUSE_SPEED = 1;
    float mouseStep = MOUSE_SPEED * Globals::deltaTime;

    static const float MIN_PITCH = -89;
    static const float MAX_PITCH = 89;

    yaw += mouseStep * Input::getMouseOffset().x;;
    pitch += mouseStep * Input::getMouseOffset().y;

    pitch = min(pitch, MAX_PITCH);
    pitch = max(pitch, MIN_PITCH);

    cout << "y: " << yaw << endl;
    cout << "p: " << pitch << endl;
    direction = glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    );

    glm::vec3 right = glm::normalize(glm::cross(direction, LOCAL_UP));
    float dirMult = 0;
    float rightMult = 0;
    if(Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)){
        dirMult = 1;
    }
    if(Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)){
        dirMult = -1;
    }
    if(Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)){
        rightMult = -1;
    }
    if(Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)){
        rightMult = 1;
    }

    shift3D += (direction * dirMult + right * rightMult) * stepVal;
}

glm::vec2 Camera::getPos2D() const {
    return pos2D;
}

glm::vec3 Camera::getPos3D() const {
    float lon = pos2D.x;
    float lat = pos2D.y;
	
	float r = RADIUS + height;
	float x = r*cos(lat)*cos(lon);
	float y = r*cos(lat)*sin(lon);
	float z = r*sin(lat);
    
    return glm::vec3(x,y,z) + shift3D;
}

void Camera::changeZoom(int dir){
    static const float MIN_ZOOM = 0.1f;
    static const float MAX_ZOOM = 100.0f;
    
    float step = MIN_ZOOM;
    if(zoom > 1){
        step = 0.5f;
    }
    else if(zoom >= 5 && zoom < 10){
        step = 1.0f;
    }
    else if(zoom >= 20){
        step = 5.0f;
    }

    zoom += dir * step;
    zoom = max(zoom, MIN_ZOOM);
    zoom = min(zoom, MAX_ZOOM);
    cout << "Zoom: " << zoom << endl;
}

void Camera::changeHeight(int dir){
    static const float MIN_HEIGHT = 0.0f;
    static const float MAX_HEIGHT = 5000.0f;
    
    float step = 500;
    height += dir * step;
    height = max(height, MIN_HEIGHT);
    height = min(height, MAX_HEIGHT);
    cout << "Height: " << height << endl;
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

glm::mat4 Camera::getVPMat(){
    return getProjectionMat() * getViewMat();
}

glm::mat4 Camera::getViewMat(){
    return glm::lookAt(
                getPos3D(), // the position of your camera, in world space
                getPos3D() + direction,   // where you want to look at, in world space
                LOCAL_UP    // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
            );
}

glm::mat4 Camera::getProjectionMat(){
    float fov = 45.0f;
    return glm::perspective(
                glm::radians(fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
                Window::getRatio(),       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
                0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
                100000.0f             // Far clipping plane. Keep as little as possible.
            );
}