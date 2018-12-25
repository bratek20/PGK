#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <memory>

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

class Camera {
    glm::vec3 position;
    float zoom;

public:
    static const glm::vec3 LOCAL_UP;

    static CameraPtr create(glm::vec3 initPos);

    void update();

    glm::vec2 getPos2D() const;
    float getZoom() const;

    glm::mat4 getViewMat();
    glm::mat4 getProjectionMat(); 

private:
    Camera(glm::vec3 initPos);
    void changeZoom(int dir);

};

#endif