#ifndef AQUARIUM_H
#define AQUARIUM_H

#include "Actor.h"

class Aquarium;
using AquariumPtr = std::shared_ptr<Aquarium>;

class Aquarium : public Actor {
    float width;
    float height;
    float depth;
    float time;

public:
    static AquariumPtr create(float width, float height, float depth);
    
    void onUpdate() override;

protected:
    Aquarium();

private:
    static ActorPtr makeWall(float width, float height, float depth, int dx, int dz);
    void spawnBubble();

};
#endif