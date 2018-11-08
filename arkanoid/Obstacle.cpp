#include "Obstacle.h"
#include "Globals.h"
#include "Shapes.h"
#include <cmath>

Obstacle::Obstacle(MeshPtr mesh) : Actor(mesh) {
    left = Shapes::createHexagon(-2, 0, 1, 1, 0, Mesh::BRICK);
    right = Shapes::createHexagon(2, 0, 1, 1, 0, Mesh::BRICK);
    top = Shapes::createHexagon(0, 2, 1, 1, 0, Mesh::BRICK);
    bottom = Shapes::createHexagon(0, -2, 1, 1, 0, Mesh::BRICK);

    left->setOnCollide([](ActorPtr a){a->destroy();});
    right->setOnCollide([](ActorPtr a){a->destroy();});
    top->setOnCollide([](ActorPtr a){a->destroy();});
    bottom->setOnCollide([](ActorPtr a){a->destroy();});
    
}

ActorPtr Obstacle::create(float x, float y, bool empty){
    auto obstacle = ObstaclePtr(new Obstacle(nullptr));
    obstacle->empty = empty;
    obstacle->setPosition(x, y);
    obstacle->setScale(0.75f, 0.75f);
    if(!empty){
        obstacle->addChilds({obstacle->left, obstacle->right, obstacle->top, obstacle->bottom});
    }
    return obstacle; 
}

void Obstacle::onUpdate(){
    if(!empty){
        time += Globals::deltaTime;
        rotate(100.0f * Globals::deltaTime);
        
        float shift = (sin(10 * time) + 1)/2;
        left->setPosition(-2 - shift, 0);
        right->setPosition(2 + shift, 0);
        top->setPosition(0, 2 + shift);
        bottom->setPosition(0, -2 - shift);
    }

    if(childs.empty()){
        destroy();
    }
}