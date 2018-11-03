#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include "Mesh.h"
#include <vector>
#include <memory>

class Actor;
using ActorPtr = std::shared_ptr<Actor>;

class Actor : public std::enable_shared_from_this<Actor> {
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
    MeshPtr mesh;
    std::weak_ptr<Actor> parent;    
    std::vector<ActorPtr> childs;

public:
    static ActorPtr create(MeshPtr mesh);
    virtual ~Actor(){}

    void update();
    void render(const glm::mat3& worldMat);

    void addChild(ActorPtr child);
    void addChilds(std::vector<ActorPtr> childs);

    void setPosition(float x, float y);
    void setScale(float scaleX, float scaleY);
    void setRotation(float rotation);

    void move(glm::vec2 dPos);
    void rotate(float dRotation);

    float getScaleX() const;
    float getScaleY() const;

    glm::vec2 getWorldPosition() const;
    std::vector<glm::vec2> getWorldCoords() const;
    
protected:
    Actor(MeshPtr mesh);
    virtual void onUpdate() {};

private:
    glm::mat3 getWorldMat() const;
    glm::mat3 getLocalMat() const;
    glm::mat3 getPositionMat() const;
    glm::mat3 getScaleMat() const;
    glm::mat3 getRotationMat() const;
};

#endif