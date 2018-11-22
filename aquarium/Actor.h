#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include "Mesh.h"
#include <vector>
#include <memory>
#include <functional>

class Actor;
using ActorPtr = std::shared_ptr<Actor>;
using ActorCallback = std::function<void(ActorPtr)>;

class Actor : public std::enable_shared_from_this<Actor> {
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
    MeshPtr mesh;   
    ActorCallback onCollideCallback;
    bool exists = true;
    bool collides = true;

public:
    static ActorPtr create(MeshPtr mesh);
    virtual ~Actor(){}

    void destroy();
    void update();
    void render(const glm::mat4& worldMat);

    void addChild(ActorPtr child);
    void addChilds(std::vector<ActorPtr> childs);

    int childsNum() const;
    
    void setPosition(float x, float y, float z);
    void setScale(float scaleX, float scaleY, float scaleZ);
    void setRotation(float rotation);

    void move(glm::vec3 dPos);
    void rotate(float dRotation);

    void setOnCollide(std::function<void(ActorPtr)> onCollide);
    void disableCollisions();

    glm::vec3 getWorldPosition() const;
    std::vector<glm::vec3> getWorldCoords() const;
    
protected:
    std::weak_ptr<Actor> parent; 
    std::vector<ActorPtr> childs;
    Actor(MeshPtr mesh);
    virtual void onUpdate() {};

private:
    void onCollide();
    
    glm::mat4 getWorldMat() const;
    glm::mat4 getLocalMat() const;
    glm::mat4 getPositionMat() const;
    glm::mat4 getScaleMat() const;
    glm::mat4 getRotationMat() const;
};

#endif