#ifndef SHAPES_H
#define SHAPES_H

#include "Actor.h"

class Shapes {
public:
    static ActorPtr createRectangle(float x, float y, float scaleX, float scaleY, float rotation, Mesh::Color color);
    static ActorPtr createWorldFrame(float worldScaleX, float worldScaleY, Mesh::Color color);
    static ActorPtr createPlatform(float x, float y, float scaleX, float scaleY, Mesh::Color color);
};

#endif