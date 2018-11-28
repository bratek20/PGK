#ifndef LIGHT_H
#define LIGHT_H

#include "Actor.h"

class Light;
using LightPtr = std::shared_ptr<Light>;

class Light : public Actor {
public:
    static LightPtr create();

protected:
    Light();
};
#endif