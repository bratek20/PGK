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

using namespace std;

bool isKeyPressed[1024];
function<void()> callbacks[1024];
void setCallback(int key, function<void()> callback)
{
    callbacks[key] = callback;
}

void onKeyClicked(GLFWwindow* , int key, int , int action, int)
{
    if (key >= 0 && key < 1024)
    {
        bool firstPressed = !isKeyPressed[key] && action == GLFW_PRESS;
        if (action == GLFW_PRESS)
        {
            isKeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isKeyPressed[key] = false;
        }
        firstPressed = true;
        if(firstPressed && callbacks[key])
        {
            callbacks[key]();
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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return true;
}

// void initInputHandlers(Actor& actor)
// {
//     setCallback(GLFW_KEY_UP, bind(&Actor::move, ref(actor), 0.0f, -0.1f));
//     setCallback(GLFW_KEY_DOWN, bind(&Actor::move, ref(actor), 0.0f, 0.1f));
//     setCallback(GLFW_KEY_LEFT, bind(&Actor::move, ref(actor), -0.1f, 0.0f));
//     setCallback(GLFW_KEY_RIGHT, bind(&Actor::move, ref(actor), 0.1f, 0.0f));
// }

int main()
{
    if(!initWindow())
    {
        return -1;
    }

    Mesh::init();
    ActorPtr world = Actor::create(nullptr); 
	BallPtr ball = Ball::create(0.0f, 1.0f);

	world->setScale(0.1f, 0.1f);
	world->addChild(ball);
	world->addChild(Shapes::createWorldFrame(world->getScaleX(), world->getScaleY(), Mesh::BLUE));

	Globals::currentFrameTime = glfwGetTime();
	Globals::deltaTime = 1.0f / 60.0f; 
	Globals::previousFrameTime = Globals::currentFrameTime - Globals::deltaTime;
	
	do
    {
		world->update();

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


