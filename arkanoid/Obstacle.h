#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Actor.h"

class Obstacle;
using ObstaclePtr = std::shared_ptr<Obstacle>;

class Obstacle : public Actor {
    float time = 0.0f;
    ActorPtr left;
    ActorPtr right;
    ActorPtr top;
    ActorPtr bottom;

public:
    static ActorPtr create(float x, float y);
    
protected:
    Obstacle(MeshPtr mesh);
    void onUpdate() override;

};

#endif