#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Mesh.h"
#include "Scene.h"
#include "Color.h"
#include "Player.h"
#include "Aquarium.h"

#include <iostream>

using namespace std;

static const float WIDTH = 60;
static const float HEIGHT = 30;
static const float DEPTH = 100;

ScenePtr scene;
PlayerPtr player;
AquariumPtr aquarium;

int cameraSetting = 0;
void changeCameraSetting(){
	auto camera = scene->getCamera();
	cameraSetting = (cameraSetting + 1)%3;
	if(cameraSetting == 0){ // out of aquarium
		scene->addChild(camera);
		camera->setPosition({-2.5f*WIDTH, HEIGHT/2, 0});
		camera->setRotation({0, 90, 0});
		aquarium->setFrontWallStatus(false);
	}
	else if(cameraSetting == 1){ // third person
		player->addChild(camera);
		camera->setPosition({0, 2.5f, -4});
		camera->setRotation({10, 0, 0});
		aquarium->setFrontWallStatus(true);
	}
	else if(cameraSetting == 2){ // first person
		player->addChild(camera);
		camera->setPosition({0, 1, 1});
		camera->setRotation({0, 0, 0});
		aquarium->setFrontWallStatus(true);
	}
}

bool shouldReset = false;
int level = 1;
int points = 0;
void resetGame(bool won){
	if(aquarium != nullptr){
		points += aquarium->getPoints();
	}
	cout << "Prev| Level: " << level << ", points: " << points << endl;
	
	shouldReset = true;
	level = won ? level + 1 : 1;
	points = won ? points : 0;
	cout << "Cur| Level: " << level << ", points: " << points << endl << endl;
}

void initGame(){
	scene = Scene::create();
	Globals::player = player = Player::create(WIDTH, HEIGHT, DEPTH, [](){resetGame(true);});

	aquarium = Aquarium::create(WIDTH, HEIGHT, DEPTH, level, [](ActorPtr){resetGame(false);});
	scene->addChilds({player, aquarium});
	
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

	resetGame(false);
	
	while(!Window::shouldClose()){
		if(shouldReset){
			initGame();
			shouldReset = false;
		}

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


