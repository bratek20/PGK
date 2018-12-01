#include "Aquarium.h"
#include "Globals.h"
#include "Bubble.h"

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

AquariumPtr Aquarium::create(float width, float height, float depth){
    auto aquarium = AquariumPtr(new Aquarium());
    aquarium->width = width;
    aquarium->height = height;
    aquarium->depth = depth;

    auto floor = Actor::create(Mesh::create(Mesh::CUBE, Colors::BLUE));
	floor->setScale({width, 1, depth});
	
	aquarium->addChilds({
		floor,
		makeWall(width, height, depth, 1, 0),
		makeWall(width, height, depth, -1, 0),
		makeWall(width, height, depth, 0, 1),
		makeWall(width, height, depth, 0, -1)
	});

    return aquarium;
}

void Aquarium::onUpdate(){
    const static float SPAWN_TIME = 0.2f;

    time += Globals::deltaTime;
    if(time > SPAWN_TIME && childsNum() < 50){
        spawnBubble();
        time = 0.0f;
    }
}

void Aquarium::spawnBubble(){
    auto random = Globals::random;
    auto bubble = Bubble::create(random(-width/2 + 2, width/2 - 2), 0, random(-depth/2 + 2, depth/2 - 2), random(1, 5));
    addChild(bubble);
}