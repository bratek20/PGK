#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <memory>

class Camera;
using CameraPtr = std::shared_ptr<Camera>;

class Camera {
    static const glm::vec3 LOCAL_UP;
    static const glm::vec2 LOCAL_UP_2D;

    glm::vec2 pos2D;
    float zoom;
    float height;
    float horAngle;
    float vertAngle;

public:
    static const float RADIUS;

    static CameraPtr create(glm::vec2 initPos);

    void update(bool is2D);

    glm::vec2 getPos2D() const;
    glm::vec3 getPos3D() const;

    float getZoom() const;
    std::pair<int,int> getCenterSegment();
    std::pair<int,int> getViewSize(float cosY);

    glm::mat4 getVPMat();

private:
    Camera(glm::vec2 initPos);

    void update2D();
    void update3D();
    
    void changeZoom(int dir);
    void changeHeight(int dir);

    glm::vec2 getDir2D() const;
    glm::vec3 getCenter() const;

    glm::vec3 convert(glm::vec2 degPos, float h) const;

    glm::mat4 getViewMat();
    glm::mat4 getProjectionMat(); 
};

#endif