#include "Player.h"
#include "Input.h"
#include "Globals.h"

Player::Player() : Actor(Mesh::create(Mesh::CUBE, Colors::RED)) {
}

PlayerPtr Player::create(){
    return PlayerPtr(new Player());
}

void Player::onUpdate(){
    static const float VELOCITY = 5;
    float step = VELOCITY * Globals::deltaTime;

    if(Input::isPressed(GLFW_KEY_W) || Input::isPressed(GLFW_KEY_UP)){
        move({0, 0, step});
    }
    if(Input::isPressed(GLFW_KEY_S) || Input::isPressed(GLFW_KEY_DOWN)){
        move({0, 0, -step});
    }
    if(Input::isPressed(GLFW_KEY_A) || Input::isPressed(GLFW_KEY_LEFT)){
        move({step, 0, 0});
    }
    if(Input::isPressed(GLFW_KEY_D) || Input::isPressed(GLFW_KEY_RIGHT)){
        move({-step, 0, 0});
    }
}