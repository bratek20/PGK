#include "Player.h"
#include "Input.h"
#include "Globals.h"
#include "Light.h"

#include <algorithm>
using namespace std;

Player::Player() : Actor(Mesh::create(Mesh::CUBE, Colors::RED)) {
}

PlayerPtr Player::create(){
    auto player = PlayerPtr(new Player());
    return player;
}


void Player::onUpdate(){
    static const float VELOCITY = 10;
    float stepVal = VELOCITY * Globals::deltaTime;
    glm::vec3 step = glm::vec3(0);

    if(Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)){
        step = {0, 0, stepVal};
    }
    if(Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)){
        step = {0, 0, -stepVal};
    }
    if(Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)){
        step = {stepVal, 0, 0};
    }
    if(Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)){
        step = {-stepVal, 0, 0};
    }

    auto newRot = getRotation() + glm::vec3(-Input::getMouseOffset().y * Globals::deltaTime, -Input::getMouseOffset().x * Globals::deltaTime, 0); 
    static const float MAX_X_DEG = 66;
    newRot.x = min(newRot.x, MAX_X_DEG);
    newRot.x = max(newRot.x, -MAX_X_DEG);
    setRotation(newRot);

    step = static_cast<glm::vec3>(getRotationMat() * glm::vec4(step, 0));
    move(step);
}