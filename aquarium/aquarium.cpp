#include "Window.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"

using namespace std;

int main(){
    if(!Window::init()){
        return -1;
    }
    Mesh::init();

	ScenePtr scene = Scene::create();

	Globals::currentFrameTime = glfwGetTime();
	Globals::deltaTime = 1.0f / 60.0f; 
	Globals::previousFrameTime = Globals::currentFrameTime - Globals::deltaTime;
	
	auto act = Actor::create(Mesh::create(Mesh::CUBE, Colors::GREEN));
	scene->addChild(act);
	
	do{
		scene->update();

        glClear( GL_COLOR_BUFFER_BIT );
		scene->render();
		glfwSwapBuffers(Window::getPtr());

		glfwPollEvents();

		Globals::previousFrameTime = Globals::currentFrameTime;
		Globals::currentFrameTime = glfwGetTime();
		Globals::deltaTime = Globals::currentFrameTime - Globals::previousFrameTime; 
	}
	while( glfwGetKey(Window::getPtr(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(Window::getPtr()) == 0 );

    Mesh::clear();
	Window::clear();

	return 0;
}


