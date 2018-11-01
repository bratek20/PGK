#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include "Mesh.h"

class Actor {
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
    Mesh mesh;

public:
    Actor(); 
    void update();
    void render();

    void setPosition(glm::vec2 position);
    void setPosition(float x, float y);
    void move(glm::vec2 deltaPos);
    void move(float dx, float dy);

    glm::mat3 getWorldMat();
    
private:
    glm::mat3 getPositionMat();
    glm::mat3 getScaleMat();
    glm::mat3 getRotationMat();
    
    
};

#endif