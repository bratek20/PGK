#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include "Mesh.h"
#include <vector>
#include <memory>
#include <functional>

class Actor;
using ActorPtr = std::shared_ptr<Actor>;
using Callback = std::function<void()>;
using ActorCallback = std::function<void(ActorPtr)>;

class Actor : public std::enable_shared_from_this<Actor> {
    ActorCallback onCollideCallback;

    bool exists = true;
    bool collides = false;
    bool isVisible = true;
    bool isInstancedBuffer = false;

public:
    static ActorPtr create(MeshPtr mesh);
    virtual ~Actor(){}

    void destroy();
    void update();
    void render(const glm::mat4& worldMat);
    void checkCollisions();
    void setVisibleStatus(bool isVisible);
    void setInstancedBufferStatus(bool isInstancedBuffer);

    void addChild(ActorPtr child);
    void addChilds(std::vector<ActorPtr> childs);

    int childsNum() const;
    std::vector<ActorPtr> getChilds() const;
    
    void setPosition(glm::vec3 position);
    void setScale(glm::vec3 scale);
    void setRotation(glm::vec3 rotation);

    void move(glm::vec3 dPos);
    void rotate(glm::vec3 dRot);

    void setOnCollide(std::function<void(ActorPtr)> onCollide);
    void setCollisionStatus(bool collides);

    glm::vec3 getLocalPosition() const;
    glm::vec3 getWorldPosition() const;
    std::vector<glm::vec3> getWorldCoords() const;
    glm::vec3 getScale() const;
    glm::vec3 getRotation() const;
    bool isDestroyed() const;
    
protected:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    MeshPtr mesh;   
    
    std::weak_ptr<Actor> parent; 
    std::vector<ActorPtr> childs;
    Actor(MeshPtr mesh);
    virtual void onUpdate() {};

    MeshPtr getMesh() const;
    glm::mat4 getWorldMat() const;
    glm::mat4 getLocalMat() const;
    glm::mat4 getPositionMat() const;
    glm::mat4 getScaleMat() const;
    glm::mat4 getRotationMat() const;

private:
    void onCollide();
};

#endif