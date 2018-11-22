#include "Window.h"
#include "Globals.h"
#include "Mesh.h"


using namespace std;

int main(){
    if(!Window::init()){
        return -1;
    }
    Mesh::init();

	Globals::currentFrameTime = glfwGetTime();
	Globals::deltaTime = 1.0f / 60.0f; 
	Globals::previousFrameTime = Globals::currentFrameTime - Globals::deltaTime;
	
	do{
        glClear( GL_COLOR_BUFFER_BIT );
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


