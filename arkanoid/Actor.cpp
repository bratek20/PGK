#include "Actor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

Actor::Actor() : position(0.0f, 0.0f), scale(1.0f, 1.0f), rotation(0.0f) {
}

void Actor::render(){
    mesh.render(getWorldMat()); 
}

glm::mat3 Actor::getWorldMat(){
    return getPositionMat() * getRotationMat() * getScaleMat();
}

void Actor::setPosition(glm::vec2 position){
    this->position = position;
}

void Actor::setPosition(float x, float y){
    setPosition(glm::vec2(x, y));
}

void Actor::move(glm::vec2 deltaPos){
    position += deltaPos;
}

void Actor::move(float dx, float dy){
    move(glm::vec2(dx, dy));
}

glm::mat3 Actor::getPositionMat(){
    glm::mat4 positionMat(1.0f);
    positionMat[2][0] = position.x;
    positionMat[2][1] = position.y;
    return positionMat;
}

glm::mat3 Actor::getScaleMat(){
    glm::mat3 scaleMat(1.0f);
    scaleMat[0][0] = scale.x;
    scaleMat[1][1] = scale.y;
    return scaleMat;
}

glm::mat3 Actor::getRotationMat(){
    glm::mat3 roatationMat(1.0f);
    float radRot = glm::radians(rotation);
    float sinRot = sin(radRot);
    float cosRot = cos(radRot);

    roatationMat[0][0] = cosRot;
    roatationMat[0][1] = -sinRot;
    roatationMat[1][0] = sinRot;
    roatationMat[1][1] = cosRot;

    return roatationMat;
}