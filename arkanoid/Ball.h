#ifndef BALL_H
#define BALL_H

#include "Actor.h"

class Ball;
using BallPtr = std::shared_ptr<Ball>;

class Ball : public Actor {
    glm::vec2 velocity;

public:
    static BallPtr create(float xVelocity, float yVelocity);
    void setVelocity(glm::vec2 velocity);
    glm::vec2 getVelocity() const;
    
protected:
    Ball(MeshPtr mesh);
    void onUpdate() override;

};

#endif