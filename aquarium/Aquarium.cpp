#include "Aquarium.h"
#include "Globals.h"
#include "Bubble.h"

#include <algorithm>

const int Aquarium::INIT_MAX_BUBLES = 30;
const int Aquarium::BUBLES_PER_LEVEL = 20;
const int Aquarium::MAX_SHINING_BUBBLES = 5;
const float Aquarium::BUBBLE_SPAWN_TIME = 0.2f;
const float Aquarium::SHINING_BUBBLE_SPAWN_CHANCE = 0.1f;
const float Aquarium::BUBBLE_MAX_SIZE = 2.5f;
const int Aquarium::POINTS_PER_BUBBLE = 10;

Aquarium::Aquarium() : Actor(nullptr) {
}

ActorPtr Aquarium::makeWall(float width, float height, float depth, int dx, int dz){
	auto wall = Actor::create(Mesh::create(Mesh::CUBE, Colors::BLUE));
    if(dx != 0){
        wall->setScale({1, height, depth});
    }
	else{
        wall->setScale({width, height, 1});
    }

	wall->move({width/2 * dx, height/2, depth/2*dz});
	return wall;
}

AquariumPtr Aquarium::create(float width, float height, float depth, int level, ActorCallback endGameCallback){
    auto aquarium = AquariumPtr(new Aquarium());
    aquarium->width = width;
    aquarium->height = height;
    aquarium->depth = depth;
    aquarium->level = level;
    aquarium->endGameCallback = endGameCallback;

    auto floor = Actor::create(Mesh::create(Mesh::CUBE, Colors::SAND));
	floor->setScale({width, 1, depth});
    floor->move({0, -0.5f, 0});
	
    aquarium->frontWall = makeWall(width, height, depth, -1, 0);
	aquarium->bubbles = Actor::create(nullptr);
    aquarium->shiningBubbles = Actor::create(nullptr);
    aquarium->addChilds({
		floor,
        aquarium->frontWall,
		makeWall(width, height, depth, 1, 0),
		makeWall(width, height, depth, 0, 1),
		makeWall(width, height, depth, 0, -1),
        aquarium->bubbles,
        aquarium->shiningBubbles
	});

    return aquarium;
}

void Aquarium::onUpdate(){
    time += Globals::deltaTime;
    if(time > BUBBLE_SPAWN_TIME / std::max(1, level/2) && bubbles->childsNum() + shiningBubbles->childsNum() < INIT_MAX_BUBLES + BUBLES_PER_LEVEL * level){
        spawnBubble();
        time = 0.0f;
    }

    for(auto child : childs){
        if(child->getLocalPosition().y > height){
            child->destroy();
        }
    }
}

void Aquarium::setFrontWallStatus(bool isEnabled){
    frontWall->setVisibleStatus(isEnabled);
}

int Aquarium::getPoints() const{
    return points;
}

void Aquarium::spawnBubble(){
    auto random = Globals::random;
    bool isShining = random(0, 1) < SHINING_BUBBLE_SPAWN_CHANCE && shiningBubbles->childsNum() < MAX_SHINING_BUBBLES;
    auto bubble = Bubble::create(random(-width/2 + 2, width/2 - 2), 0, random(-depth/2 + 2, depth/2 - 2), random(1, BUBBLE_MAX_SIZE), isShining);
    if(isShining){
        bubble->setOnCollide([this](ActorPtr a){
                if(a->isDestroyed()){
                    points += POINTS_PER_BUBBLE;
                }
                a->destroy();
                
            });
        shiningBubbles->addChild(bubble);
    }
    else{
        bubbles->addChild(bubble);
        bubble->setOnCollide(endGameCallback);
    }
    
    addChild(bubble);
}