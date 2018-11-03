#include "Ball.h"
#include "Globals.h"

Ball::Ball(MeshPtr mesh) : Actor(mesh) {}

BallPtr Ball::create(float xVelocity, float yVelocity){
    auto mesh = Mesh::create(Mesh::EQUILATERAL, Mesh::RED);
    auto ball = BallPtr(new Ball(mesh));
    ball->setVelocity({xVelocity, yVelocity});
    return ball; 
}

void Ball::setVelocity(glm::vec2 velocity){
    this->velocity = velocity;
}

glm::vec2 Ball::getVelocity() const{
    return velocity;
}

void Ball::onUpdate(){
    move(velocity * Globals::deltaTime);
    rotate(180.0f * Globals::deltaTime);
}