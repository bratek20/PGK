#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"
#include "Player.h"
#include "DataReader.h"

#include <iostream>
#include <algorithm>

using namespace std;

ScenePtr scene;
PlayerPtr player;
glm::vec3 thirdPersonCamPos = {0, 2.5f, -4};
int cameraSetting = 0;
void changeCameraSetting(){
	auto camera = scene->getCamera();
	cameraSetting = (cameraSetting + 1)%2;
	if(cameraSetting == 0){ // third person
		player->addChild(camera);
		camera->setPosition(thirdPersonCamPos);
		camera->setRotation({10, 0, 0});
	}
	else if(cameraSetting == 1){ // first person
		player->addChild(camera);
		camera->setPosition({0, 1, 1});
		camera->setRotation({0, 0, 0});
	}
}

float zoomValue = 1;
void zoomCamera(){
	
	if(cameraSetting == 0){ // third person
		scene->getCamera()->setPosition(thirdPersonCamPos * zoomValue);
	}
}

void zoomIn(){
	zoomValue -= 0.25f;
	zoomValue = max(1.0f, zoomValue);
	zoomCamera();
}

void zoomOut(){
	zoomValue += 0.25f;
	zoomValue = min(2.0f, zoomValue);
	zoomCamera();
}

void initGame(){
	scene = Scene::create();
	Globals::player = player = Player::create();
	scene->addChild(player);
	cameraSetting = 0;
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
	Input::onKeyPressed(GLFW_KEY_O, zoomIn);
	Input::onKeyPressed(GLFW_KEY_P, zoomOut);
	
	DataReader::read("data/N50E016.hgt");
	
	initGame();
	while(!Window::shouldClose()){
		Input::handle();

		scene->update();

        Window::clear();
		Mesh::applyPlayerPosition(player->getWorldPosition());
		scene->render();
		Window::swapBuffers();
		
		Globals::updateTime();
	}

    Mesh::clear();
	Window::close();
	return 0;
}


