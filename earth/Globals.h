#ifndef GLOBALS_H
#define GLOBALS_H

#include <memory>

class Actor;
using ActorPtr = std::shared_ptr<Actor>;

class Globals{
private:
    static int LOD;
    static float secFragTime;   
    static int frames;

public:
    static float previousFrameTime;
    static float currentFrameTime;
    static float deltaTime;
    static unsigned renderedVertices;

    static void init();
    static void update();

    static float random(float a = 0.0f, float b = 1.0f);
    static int getLOD();    
};

#endif