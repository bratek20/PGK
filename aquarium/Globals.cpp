#include "Globals.h"

#include <GLFW/glfw3.h>

float Globals::previousFrameTime;
float Globals::currentFrameTime;
float Globals::deltaTime;

void Globals::init(){
    Globals::currentFrameTime = glfwGetTime();
	Globals::deltaTime = 1.0f / 60.0f; 
	Globals::previousFrameTime = Globals::currentFrameTime - Globals::deltaTime;
}

void Globals::updateTime(){
    Globals::previousFrameTime = Globals::currentFrameTime;
    Globals::currentFrameTime = glfwGetTime();
    Globals::deltaTime = Globals::currentFrameTime - Globals::previousFrameTime; 
}