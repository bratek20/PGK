#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <memory>

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

class Camera {
    static const glm::vec3 LOCAL_UP;

    glm::vec2 pos2D;
    float zoom;
    float height;
    glm::vec3 direction;
    float horAngle;
    float vertAngle;

public:
    static const float RADIUS;

    static CameraPtr create(glm::vec2 initPos);

    void update();

    glm::vec2 getPos2D() const;
    glm::vec3 getPos3D() const;
    float getZoom() const;
    std::pair<int,int> getCenterSegment();
    std::pair<int,int> getViewSize();

    glm::mat4 getVPMat();

private:
    Camera(glm::vec2 initPos);

    void changeZoom(int dir);
    void changeHeight(int dir);

    glm::mat4 getViewMat();
    glm::mat4 getProjectionMat(); 
};

#endif