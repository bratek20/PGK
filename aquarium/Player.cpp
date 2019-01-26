#include "Player.h"
#include "Input.h"
#include "Globals.h"
#include "Light.h"
#include "Assets.h"

#include <algorithm>
using namespace std;

Player::Player() : Actor(Mesh::create(Assets::OBJ, Colors::RED)) {
}

PlayerPtr Player::create(float aqWidth, float aqHeight, float aqDepth, Callback winGameCallback){
    auto player = PlayerPtr(new Player());
    player->aqWidth = aqWidth * 0.95f;
    player->aqHeight = aqHeight * 0.95f;
    player->aqDepth = aqDepth * 0.95f;
    player->winGameCallback = winGameCallback;
    player->setPosition({0, aqHeight/2, -aqDepth/2 * 0.85f});

    auto light = Light::create(5, Colors::RED, {0, 1, 0});
    player->addChild(light);

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

    auto pos = getLocalPosition();
    if(pos.z > aqDepth/2){
        winGameCallback();
    }

    pos.x = min(pos.x, aqWidth/2);
    pos.x = max(pos.x, -aqWidth/2);

    pos.y = min(pos.y, aqHeight);
    pos.y = max(pos.y, 0.0f);

    pos.z = max(pos.z, -aqDepth/2);
    
    setPosition(pos);
}