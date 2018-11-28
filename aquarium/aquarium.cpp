#include "Window.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"

using namespace std;

int main(){
    if(!Window::open()){
        return -1;
    }
    Mesh::init();

	ScenePtr scene = Scene::create();

	Globals::currentFrameTime = glfwGetTime();
	Globals::deltaTime = 1.0f / 60.0f; 
	Globals::previousFrameTime = Globals::currentFrameTime - Globals::deltaTime;
	
	auto sphere = Actor::create(Mesh::create(Mesh::SPHERE, Colors::GREEN));
	sphere->move({2, 0.5f,0});
	sphere->setScale(2,3,4);
	scene->addChild(sphere);

	auto cube = Actor::create(Mesh::create(Mesh::CUBE, Colors::RED));
	cube->move({-2, 0, 1});
	cube->setScale(1,3,2);
	scene->addChild(cube);
	
	do{
		scene->update();

        Window::clear();
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
	Window::close();

	return 0;
}


