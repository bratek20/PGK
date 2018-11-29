#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"
#include "Player.h"

using namespace std;

int main(){
    if(!Window::open()){
        return -1;
    }
	Input::init();
    Mesh::init();
	Globals::init();
	ScenePtr scene = Scene::create();
	
	auto floor = Actor::create(Mesh::create(Mesh::CUBE, Colors::BROWN));
	floor->setScale({100,1,100});
	floor->move({0,-1,0});

	scene->addChild(floor); 
	auto s1 = Actor::create(Mesh::create(Mesh::SPHERE, Colors::GREEN));
	s1->move({2, 0.5f,0});
	//sphere->setScale(2,3,4);
	scene->addChild(s1);

	auto s2 = Actor::create(Mesh::create(Mesh::SPHERE, Colors::GREEN));
	s2->move({-2, 0.5f,0});
	//sphere->setScale(2,3,4);
	scene->addChild(s2);

	auto player = Player::create();
	player->addChild(scene->getCamera());
	scene->addChild(player);
	
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


