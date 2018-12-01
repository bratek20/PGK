#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"
#include "Player.h"
#include "Aquarium.h"

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

void initGame(){
	static const float WIDTH = 60;
	static const float HEIGHT = 30;
	static const float DEPTH = 100;

	scene = Scene::create();
	player = Player::create();
	player->setPosition({0, HEIGHT/2, -DEPTH/2 * 0.8f});

	scene->addChild(player);
	scene->addChild(Aquarium::create(WIDTH, HEIGHT, DEPTH));
	changeCameraSetting();
}

int main(){
    if(!Window::open()){
        return -1;
    }
	Input::init();
    Mesh::init();
	Globals::init();
	
	Input::onKeyPressed(GLFW_KEY_TAB, changeCameraSetting);

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


