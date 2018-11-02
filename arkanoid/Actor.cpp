#include "Actor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

Actor::Actor() : position(0.0f, 0.0f), scale(1.0f, 1.0f), rotation(0.0f) {
}

ActorPtr Actor::create(MeshPtr mesh){
    auto actor = ActorPtr(new Actor());
    actor->mesh = mesh;
    return actor;
}

void Actor::update(){
    for(auto& c : childs){
        c->update();
    }
}

void Actor::render(const glm::mat3& worldMat){
    auto myWorldMat = worldMat * getLocalMat();
    if(mesh != nullptr){
        mesh->render(myWorldMat);
    }

    for(auto& c : childs){
        c->render(myWorldMat);
    }
}

void Actor::addChild(ActorPtr child){
    childs.push_back(child);
}

void Actor::addChilds(std::vector<ActorPtr> childs){
    this->childs.insert(this->childs.end(), childs.begin(), childs.end());
}

glm::mat3 Actor::getLocalMat(){
    return getPositionMat() * getRotationMat() * getScaleMat();
}

void Actor::setPosition(float x, float y){
    position = glm::vec2(x, y);
}

void Actor::move(float dx, float dy){
    position += glm::vec2(dx, dy);
}

void Actor::setScale(float scaleX, float scaleY){
    scale = glm::vec2(scaleX, scaleY);
}

void Actor::setRotation(float rotation){
    this->rotation = rotation;   
}

float Actor::getScaleX() const{
    return scale.x;
}

float Actor::getScaleY() const{
    return scale.y;
};

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