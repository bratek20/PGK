#include "Globals.h"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

int Globals::LOD;
float Globals::secFragTime;
int Globals::frames;
unsigned Globals::renderedVertices;

float Globals::previousFrameTime;
float Globals::currentFrameTime;
float Globals::deltaTime;

void Globals::init(){
    currentFrameTime = glfwGetTime();
	deltaTime = 1.0f / 60.0f; 
	previousFrameTime = currentFrameTime - deltaTime;

    LOD = 1;
    secFragTime = 0;
    frames = 0;
    renderedVertices = 0;
}

void Globals::update(){
    static const int DEC_LOD_AT = 30;
    static const int INC_LOD_AT = 60;

    previousFrameTime = Globals::currentFrameTime;
    currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - previousFrameTime;

    frames++;
    secFragTime += deltaTime;
    if(secFragTime > 1){
        secFragTime = 0;
        if(frames < DEC_LOD_AT){
            LOD = max(1, LOD - 1);
        }
        if(frames > INC_LOD_AT){
            LOD = min(5, LOD + 1);
        }

        cout << "Vertices per sec: " << renderedVertices << "\n";
        cout << "FPS: " << frames << "\n";
        cout << "LOD: " << LOD << endl;
        
        frames = 0;
        renderedVertices = 0;
    } 
}

float Globals::random(float a, float b){
    return a + (b - a) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

int Globals::getLOD(){
    return LOD;
}