#include "Camera.h"

Camera::Camera() : Actor(nullptr){
}

CameraPtr Camera::create(){
    return CameraPtr(new Camera());
}

glm::mat4 Camera::getViewMat(){
    return glm::lookAt(
                getWorldPosition(), // the position of your camera, in world space
                glm::vec3(0,0,0),   // where you want to look at, in world space
                glm::vec3(0,1,0)       // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
            );
}

glm::mat4 Camera::getProjectionMat(){
    float fov = 45.0f;
    return glm::perspective(
                glm::radians(fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
                4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
                0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
                100.0f             // Far clipping plane. Keep as little as possible.
            );
}