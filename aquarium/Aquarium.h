#ifndef AQUARIUM_H
#define AQUARIUM_H

#include "Actor.h"

class Aquarium;
using AquariumPtr = std::shared_ptr<Aquarium>;

class Aquarium : public Actor {
    static const int INIT_MAX_BUBLES;
    static const int BUBLES_PER_LEVEL;
    static const int MAX_SHINING_BUBBLES;
    static const float BUBBLE_SPAWN_TIME;
    static const float SHINING_BUBBLE_SPAWN_CHANCE;
    static const float BUBBLE_MAX_SIZE;
    static const int POINTS_PER_BUBBLE;

    float width;
    float height;
    float depth;
    int level;
    float time = 0;
    int points = 0;
    ActorCallback endGameCallback;
    ActorPtr frontWall;
    ActorPtr bubbles;
    ActorPtr shiningBubbles;
    
public:
    static AquariumPtr create(float width, float height, float depth, int level, ActorCallback endGameCallback);
    
    void onUpdate() override;
    void setFrontWallStatus(bool isEnabled);
    int getPoints() const;

protected:
    Aquarium();

private:
    static ActorPtr makeWall(float width, float height, float depth, int dx, int dz);
    void spawnBubble();

};
#endif