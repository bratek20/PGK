#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"

class Player;
using PlayerPtr = std::shared_ptr<Player>;

class Player : public Actor {
    float aqWidth;
    float aqHeight;
    float aqDepth;
    Callback winGameCallback;

public:
    static PlayerPtr create(float aqWidth, float aqHeight, float aqDepth, Callback winGameCallback);
    
protected:
    Player();

    void onUpdate() override;
};
#endif