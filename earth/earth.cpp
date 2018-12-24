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
	string dataPath = argv[1];
	if(argc == 2){
		cout << "Reading all files in folder does not supported!" << endl;
		return false;
	}
	else{
		int wBeg = 0, wEnd = 0, lBeg = 0, lEnd = 0;
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
			idx += 3;
		}
		map = Map(dataPath, wBeg, wEnd, lBeg, lEnd); 
	}
	return true;
}

int main(int argc, char* argv[]){
    if(!Window::open()){
        return -1;
    }
	Input::init();
	Globals::init();
	MapSegment::init();

	if(!initMap(argc, argv)){
		return 1;
	}	
	
	while(!Window::shouldClose()){
		Input::handle();

        Window::clear();
		map.render();
		Window::swapBuffers();
		
		Globals::updateTime();
	}

    MapSegment::clear();
	Window::close();
	return 0;
}

