#include "Ball.h"
#include "Globals.h"

Ball::Ball(MeshPtr mesh) : Actor(mesh) {}

BallPtr Ball::create(float xVelocity, float yVelocity){
    auto mesh = Mesh::create(Mesh::EQUILATERAL, Mesh::RED);
    auto ball = BallPtr(new Ball(mesh));
    ball->velocity = glm::vec2(xVelocity, yVelocity);

    return ball; 
}

void Ball::onUpdate(){
    move(velocity * Globals::deltaTime);
}