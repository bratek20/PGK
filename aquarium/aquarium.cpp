#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"
#include "Player.h"

using namespace std;

ScenePtr scene;
PlayerPtr player;

int cameraSetting = 0;
void changeCameraSetting(){
	auto camera = scene->getCamera();
	cameraSetting = (cameraSetting + 1)%3;
	if(cameraSetting == 0){ // out of aquarium
		scene->addChild(camera);
		camera->setPosition({0, 4, -10});
		camera->setRotation({0, 0, 0});
	}
	else if(cameraSetting == 1){ // third person
		player->addChild(camera);
		camera->setPosition({0, 2, -3});
		camera->setRotation({-10, 0, 0});
	}
	else if(cameraSetting == 2){ // first person
		player->addChild(camera);
		camera->setPosition({0, 1, 1});
		camera->setRotation({0, 0, 0});
	}
}

ActorPtr makeAquariumWall(float size, float height, int dx, int dz){
	auto wall = Actor::create(Mesh::create(Mesh::CUBE, Colors::BLUE));
	wall->setScale({size, height, 1});
	wall->move({size/2 * dx, 0, size/2*dz});
	if(dx != 0){
		wall->rotate({0, 90, 0});
	}
	return wall;
}

ActorPtr makeAquarium(float size, float height){
	auto aquarium = Actor::create(nullptr);

	auto floor = Actor::create(Mesh::create(Mesh::CUBE, Colors::BLUE));
	floor->setScale({size,1,size});
	
	aquarium->addChilds({
		floor,
		makeAquariumWall(size, height, 1, 0),
		makeAquariumWall(size, height, -1, 0),
		makeAquariumWall(size, height, 0, 1),
		makeAquariumWall(size, height, 0, -1),
	});

	return aquarium;
}

void initGame(){
	scene = Scene::create();
	player = Player::create();

	

	scene->addChild(makeAquarium(100, 33)); 
	auto s1 = Actor::create(Mesh::create(Mesh::SPHERE, Colors::GREEN));
	s1->move({2, 0.5f,0});
	//sphere->setScale(2,3,4);
	scene->addChild(s1);

	auto s2 = Actor::create(Mesh::create(Mesh::SPHERE, Colors::GREEN));
	s2->move({-2, 0.5f,0});
	//sphere->setScale(2,3,4);
	scene->addChild(s2);

	player->addChild(scene->getCamera());
	scene->addChild(player);

	changeCameraSetting();
}

int main(){
    if(!Window::open()){
        return -1;
    }
	Input::init();
    Mesh::init();
	Globals::init();
	
	Input::onKeyPressed(GLFW_KEY_C, changeCameraSetting);

	initGame();
	
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


