#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>

#include <cmath>
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

#include "Actor.h"
#include "Mesh.h"
#include "Shapes.h"
#include "Globals.h"
#include "Ball.h"
#include "Obstacle.h"

using namespace std;

bool isKeyPressed[1024];
void onKeyClicked(GLFWwindow* , int key, int , int action, int)
{
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            isKeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isKeyPressed[key] = false;
        }
    }
}

bool initWindow()
{
    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Memeory", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(window, onKeyClicked);

	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

    return true;
}

bool isRunning = true;
void handleInput(ActorPtr platform)
{
    if(isKeyPressed[GLFW_KEY_SPACE])
	{
		isRunning = !isRunning;
	}

	static const int verticalShift = 15;
	if(isKeyPressed[GLFW_KEY_LEFT])
	{
		platform->move(glm::vec2(-verticalShift,0) * Globals::deltaTime);
	}
	if(isKeyPressed[GLFW_KEY_RIGHT])
	{
		platform->move(glm::vec2(verticalShift,0) * Globals::deltaTime);
	}
}

ActorPtr world;
BallPtr ball;
ActorPtr platform;
ActorPtr obstacles;

bool restart = false;
void restartGame(){
	cout << "Restarting game..." << endl;
	restart = true;
}

void createWorld(){
	restart = false;
	isRunning = false;

	world = Actor::create(nullptr); 
	ball = Ball::create(3, 8);
	ball->setPosition(0, -6);

	Globals::ball = ball;
	platform = Shapes::createPlatform(0, -7, 2, 1, Mesh::GREEN);

	world->setScale(0.1f, 0.1f);
	world->addChild(Shapes::createBackground());
	world->addChild(ball);
	world->addChild(Shapes::createWorldFrame(world->getScaleX(), world->getScaleY(), Mesh::BLUE, restartGame));

	world->addChild(platform);

	obstacles = Actor::create(nullptr);
	obstacles->addChild(Obstacle::create(-5, 0));
	obstacles->addChild(Obstacle::create(5, 0));
	
	world->addChild(obstacles);
}

int main(){
    if(!initWindow()){
        return -1;
    }

    Mesh::init();
    restartGame();

	Globals::currentFrameTime = glfwGetTime();
	Globals::deltaTime = 1.0f / 60.0f; 
	Globals::previousFrameTime = Globals::currentFrameTime - Globals::deltaTime;
	do{
		if(restart){
			createWorld();
		}

		handleInput(platform);

		if(isRunning){		
			world->update();
		}
		
		if(obstacles->childsNum() == 1){
			world->setRotation(180.0f);
		}

        glClear( GL_COLOR_BUFFER_BIT );
        world->render(glm::mat3(1.0f));
		glfwSwapBuffers(window);

		glfwPollEvents();

		Globals::previousFrameTime = Globals::currentFrameTime;
		Globals::currentFrameTime = glfwGetTime();
		Globals::deltaTime = Globals::currentFrameTime - Globals::previousFrameTime; 
	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    Mesh::clear();
	glfwTerminate();

	return 0;
}


