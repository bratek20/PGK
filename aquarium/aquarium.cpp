#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"

using namespace std;

int main(){
    if(!Window::open()){
        return -1;
    }
	Input::init();
    Mesh::init();
	Globals::init();
	ScenePtr scene = Scene::create();
	
	auto sphere = Actor::create(Mesh::create(Mesh::SPHERE, Colors::GREEN));
	sphere->move({2, 0.5f,0});
	sphere->setScale(2,3,4);
	scene->addChild(sphere);

	auto cube = Actor::create(Mesh::create(Mesh::CUBE, Colors::RED));
	cube->move({-2, 0, 1});
	cube->setScale(1,3,2);
	scene->addChild(cube);
	
	while(!Window::shouldClose()){
		Input::handle();

		scene->update();

        Window::clear();
		scene->render();
		Window::swapBuffers();
		
		Globals::updateTime();
	}

    Mesh::clear();
	Window::close();
	return 0;
}


