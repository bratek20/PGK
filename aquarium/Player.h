#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"

class Player;
using PlayerPtr = std::shared_ptr<Player>;

class Player : public Actor {
public:
    static PlayerPtr create();
    
protected:
    Player();

    void onUpdate() override;
};
#endif