#ifndef SHAPES_H
#define SHAPES_H

#include "Actor.h"

class Shapes {
public:
    static ActorPtr createRectangle(float x, float y, float scaleX, float scaleY, float rotation, Mesh::Color color);
    static ActorPtr createSubHexagon(float x, float y, float scaleX, float scaleY, float rotation, Mesh::Color color);
    static ActorPtr createHexagon(float x, float y, float scaleX, float scaleY, float rotation, Mesh::Color color);
    static ActorPtr createWorldFrame(float worldScaleX, float worldScaleY, Mesh::Color color, std::function<void()> endGameCallback);
    static ActorPtr createBackground();
    static ActorPtr createPlatform(float x, float y, float scaleX, float scaleY, Mesh::Color color);
};

#endif