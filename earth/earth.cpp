#include "Window.h"
#include "Input.h"
#include "Globals.h"
#include "Map.h"
#include "MapSegment.h"

#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

Map map;
bool initMap(int argc, char* argv[]){
	if(argc == 1){
		cout << "No data path passed" << endl;
		return false;
	}

	string dataPath = argv[1];
	if(argc == 2){
		map = Map(dataPath);
	}
	else{
		int wBeg = 0, wEnd = 0, lBeg = 0, lEnd = 0;
		int wStart = 0, lStart = 0;
		bool setStartPos = false;
		int idx = 2;
		while(idx < argc){
			string arg = string(argv[idx]); 
			if(arg == "-sz"){
				wBeg = atoi(argv[idx+1]);
				wEnd = atoi(argv[idx+2]);
			}
			else if(arg == "-dl"){
				lBeg = atoi(argv[idx+1]);
				lEnd = atoi(argv[idx+2]);
			}
			else if(arg == "-sp"){
				wStart = atoi(argv[idx+1]);
				lStart = atoi(argv[idx+2]);
				setStartPos = true;
			}
			idx += 3;
		}
		
		if(setStartPos){
			map = Map(dataPath, wStart, lStart);
		}
		else{
			map = Map(dataPath, wBeg, wEnd, lBeg, lEnd);
		}
		 
	}
	return true;
}

bool is2D = true;
void changeRenderType(){
	is2D = !is2D;
}

int main(int argc, char* argv[]){
    if(!Window::open("earth")){
        return -1;
    }
	Input::init();
	Globals::init();
	MapSegment::init();

	if(!initMap(argc, argv)){
		return 1;
	}	
	Input::onKeyPressed(GLFW_KEY_M, changeRenderType);

	while(!Window::shouldClose()){
		Input::handle();

        Window::clear();
		map.render(is2D);
		Window::swapBuffers();
		
		Globals::update();
	}

    MapSegment::clear();
	Window::close();
	return 0;
}


