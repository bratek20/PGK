#include "Actor.h"
#include "Collider.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>
using namespace std;

Actor::Actor(MeshPtr mesh) : position(0.0f, 0.0f), scale(1.0f, 1.0f), rotation(0.0f), mesh(mesh) {
}

ActorPtr Actor::create(MeshPtr mesh){
    auto actor = ActorPtr(new Actor(mesh));
    return actor;
}

void Actor::destroy(){
    exists = false;
}

void Actor::update(){
    for(auto& c : childs){
        c->update();
    }

    onUpdate();

    childs.erase(
        std::remove_if(childs.begin(), childs.end(), [](ActorPtr a){return !a->exists;}),
        childs.end());
}

void Actor::render(const glm::mat3& worldMat){
    auto myWorldMat = worldMat * getLocalMat();
    if(mesh != nullptr){
        mesh->render(myWorldMat);
    }

    for(auto& c : childs){
        c->render(myWorldMat);
    }

    if(mesh != nullptr && collides){
        if(Collider::collide(mesh->getWorldCoords(myWorldMat))){
           onCollide();
        }
    }
}

void Actor::addChild(ActorPtr child){
    child->parent = shared_from_this();
    childs.push_back(child);
}

void Actor::addChilds(std::vector<ActorPtr> childs){
    for(auto newChild : childs){
        addChild(newChild);
    }
}

int Actor::childsNum() const{
    return childs.size();
}

void Actor::setPosition(float x, float y){
    position = glm::vec2(x, y);
}

void Actor::setScale(float scaleX, float scaleY){
    scale = glm::vec2(scaleX, scaleY);
}

void Actor::setRotation(float rotation){
    this->rotation = rotation;   
}

void Actor::move(glm::vec2 dPos){
    position += dPos;
}

void Actor::rotate(float dRotation){
    rotation += dRotation;
}

float Actor::getScaleX() const{
    return scale.x;
}

float Actor::getScaleY() const{
    return scale.y;
};

void Actor::setOnCollide(std::function<void(ActorPtr)> onCollide){
    onCollideCallback = onCollide;
}

void Actor::disableCollisions(){
    collides = false;
}


void Actor::onCollide(){
    if(onCollideCallback){
        onCollideCallback(shared_from_this());
    }
    else if(!parent.expired()){
        parent.lock()->onCollide();
    }
}

glm::vec2 Actor::getWorldPosition() const{
    return getWorldMat() * glm::vec3(position, 1.0f);
}

std::vector<glm::vec2> Actor::getWorldCoords() const{
    if(mesh != nullptr){
        return mesh->getWorldCoords(getWorldMat());
    }
    return {getWorldPosition()};
}

glm::mat3 Actor::getWorldMat() const{
    if(parent.expired()){
        return getLocalMat();
    }
    return parent.lock()->getWorldMat() * getLocalMat();
}

glm::mat3 Actor::getLocalMat() const{
    return getPositionMat() * getRotationMat() * getScaleMat();
}

glm::mat3 Actor::getPositionMat() const{
    glm::mat4 positionMat(1.0f);
    positionMat[2][0] = position.x;
    positionMat[2][1] = position.y;
    return positionMat;
}

glm::mat3 Actor::getScaleMat() const{
    glm::mat3 scaleMat(1.0f);
    scaleMat[0][0] = scale.x;
    scaleMat[1][1] = scale.y;
    return scaleMat;
}

glm::mat3 Actor::getRotationMat() const{
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