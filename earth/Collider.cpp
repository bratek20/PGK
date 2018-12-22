#include "Collider.h"
#include "Globals.h"
#include <algorithm>
#include <iostream>

bool Collider::collide(ActorPtr actor){
    auto player = Globals::player;
    float dist = glm::distance(actor->getWorldPosition(), player->getWorldPosition());
    float rSum = player->getScale().x + actor->getScale().x;
    return dist <= rSum;
}
