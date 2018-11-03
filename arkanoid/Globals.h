#ifndef GLOBALS_H
#define GLOBALS_H

#include <memory>

class Ball;

class Globals{
public:
    static float previousFrameTime;
    static float currentFrameTime;
    static float deltaTime;

    static std::shared_ptr<Ball> ball;
};

#endif