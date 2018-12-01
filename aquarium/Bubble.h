#ifndef BUBBLE_H
#define BUBBLE_H

#include "Actor.h"

class Bubble;
using BubblePtr = std::shared_ptr<Bubble>;

class Bubble : public Actor {
    float scale;
    
public:
    static BubblePtr create(float x, float y, float z, float scale);

    void onUpdate() override;

protected:
    Bubble();
};
#endif