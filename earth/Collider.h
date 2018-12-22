#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <vector>
#include <functional>

#include "Actor.h"

class Collider{
public:
    static bool collide(ActorPtr actor);
};

#endif