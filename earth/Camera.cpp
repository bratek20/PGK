#include "Camera.h"
#include "Input.h"
#include "Globals.h"
#include "Window.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <functional>
#include <algorithm>
#include <cmath>

using namespace std;

const glm::vec3 Camera::LOCAL_UP = glm::vec3(0,1,0);
const glm::vec2 Camera::LOCAL_UP_2D = glm::vec2(0,1);
const float Camera::RADIUS = 6371;

Camera::Camera(glm::vec2 initPos) : pos2D(initPos), zoom(1.0f), height(1000.0f), horAngle(0.0f), vertAngle(0.0f) {
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
    static const float VELOCITY = 1;
    float stepVal = VELOCITY * Globals::deltaTime;
    
    static const float MOUSE_SPEED = 1;
    float mouseStep = MOUSE_SPEED * Globals::deltaTime;

    static const float MIN_VERT_ANGLE = -80;
    static const float MAX_VERT_ANGLE = 30;

    horAngle += mouseStep * Input::getMouseOffset().x;
    vertAngle += mouseStep * Input::getMouseOffset().y;

    vertAngle = min(vertAngle, MAX_VERT_ANGLE);
    vertAngle = max(vertAngle, MIN_VERT_ANGLE);

    glm::vec2 dir2D = getDir2D();
    glm::vec3 forward = glm::normalize(glm::vec3(dir2D.x, 0, dir2D.y));
    glm::vec3 right = glm::normalize(glm::cross(forward, LOCAL_UP));
    
    float forMult = 0;
    float rightMult = 0;
    if(Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)){
        forMult = 1;
    }
    if(Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)){
        forMult = -1;
    }
    if(Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)){
        rightMult = 1;
    }
    if(Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)){
        rightMult = -1;
    }

    glm::vec3 step = (forward * forMult + right * rightMult) * stepVal;
    pos2D += glm::vec2(step.x, step.z);
}

glm::vec2 Camera::getPos2D() const {
    return pos2D;
}

glm::vec3 Camera::getPos3D() const {
    return convert(pos2D, height);
}

glm::vec3 Camera::convert(glm::vec2 degPos, float h) const {
    float lon = glm::radians(degPos.x);
    float lat = glm::radians(degPos.y);
	
	float r = RADIUS + h / 100; // RADIUS is in km, h is in m, makes heights 10 times bigger
	float x = r*cos(lat)*cos(lon);
	float y = r*cos(lat)*sin(lon);
	float z = r*sin(lat);

    return glm::vec3(x,y,z);
}

glm::vec2 Camera::getDir2D() const {
    return glm::normalize(glm::rotate(LOCAL_UP_2D, glm::radians(-horAngle)));
}

glm::vec3 Camera::getCenter() const {
    return convert(pos2D + getDir2D(), height + 100 * vertAngle);
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
    
    float step = (MAX_HEIGHT - MIN_HEIGHT) / 10;
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

std::pair<int,int> Camera::getViewSize(float cosY){
    float width = 2.0f / (zoom * Window::getRatio() * cosY);
    float height = 2.0f / zoom;
    return {ceil(width) + 1, ceil(height) + 1};
}

glm::mat4 Camera::getVPMat(){
    return getProjectionMat() * getViewMat();
}

glm::mat4 Camera::getViewMat(){
    return glm::lookAt(
                getPos3D(), // the position of your camera, in world space
                getCenter(),   // where you want to look at, in world space
                glm::normalize(getPos3D())    // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
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