#ifndef GLOBALS_H
#define GLOBALS_H

class Globals{
public:
    static float previousFrameTime;
    static float currentFrameTime;
    static float deltaTime;

    static void init();
    static void updateTime();
};

#endif