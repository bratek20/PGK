#include "Bubble.h"
#include "Globals.h"
#include "Color.h"
#include "Light.h"

Bubble::Bubble() : Actor(Mesh::create(Mesh::SPHERE, Color::random())) {
}

BubblePtr Bubble::create(float x, float y, float z, float scale){
    auto bubble = BubblePtr(new Bubble());
    bubble->scale = scale;
    bubble->move({x, y, z});
    bubble->setScale(glm::vec3(1,1,1) * scale);
    bubble->setCollisionStatus(true);
    if(scale < 1.2f){
        auto light = Light::create(0.75f, bubble->mesh->getColor(), {0, 0.1f, 0});
        bubble->addChild(light);
    }
    return bubble;
}

void Bubble::onUpdate(){
    static const glm::vec3 VELOCITY = glm::vec3(0, 5, 0);
    static const float SCALE_FACTOR = 0.1f;
    static const glm::vec3 SCALE = glm::vec3(1, 1, 1) * SCALE_FACTOR;

    move(VELOCITY * Globals::deltaTime);
    if(getScale().x < 2*scale){
        setScale(getScale() + scale * SCALE * Globals::deltaTime);
    }
}