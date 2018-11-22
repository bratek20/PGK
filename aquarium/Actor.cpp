#include "Actor.h"
#include "Collider.h"
#include <algorithm>
#include <iostream>
using namespace std;

Actor::Actor(MeshPtr mesh) : position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation(0.0f), mesh(mesh) {
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

void Actor::render(const glm::mat4& worldMat){
    auto myWorldMat = worldMat * getLocalMat();
    if(mesh != nullptr){
        mesh->render(myWorldMat);
    }

    for(auto& c : childs){
        c->render(myWorldMat);
    }

    // if(mesh != nullptr && collides){
    //     if(Collider::collide(mesh->getWorldCoords(myWorldMat))){
    //        onCollide();
    //     }
    // }
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

void Actor::setPosition(float x, float y, float z){
    position = glm::vec3(x, y, z);
}

void Actor::setScale(float scaleX, float scaleY, float scaleZ){
    scale = glm::vec3(scaleX, scaleY, scaleZ);
}

void Actor::setRotation(float rotation){
    this->rotation = rotation;   
}

void Actor::move(glm::vec3 dPos){
    position += dPos;
}

void Actor::rotate(float dRotation){
    rotation += dRotation;
}

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

glm::vec3 Actor::getWorldPosition() const{
    return getWorldMat() * glm::vec4(position, 1.0f);
}

std::vector<glm::vec3> Actor::getWorldCoords() const{
    if(mesh != nullptr){
        return mesh->getWorldCoords(getWorldMat());
    }
    return {getWorldPosition()};
}

glm::mat4 Actor::getWorldMat() const{
    if(parent.expired()){
        return getLocalMat();
    }
    return parent.lock()->getWorldMat() * getLocalMat();
}

glm::mat4 Actor::getLocalMat() const{
    return getPositionMat() * getRotationMat() * getScaleMat();
}

glm::mat4 Actor::getPositionMat() const{
    return glm::translate(glm::mat4(1.0f), position);
}

glm::mat4 Actor::getScaleMat() const{
    return glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 Actor::getRotationMat() const{
    glm::mat4 roatationMat(1.0f);
    return roatationMat;
}