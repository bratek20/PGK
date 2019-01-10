#version 330 core

uniform int Empty;

in float ht;

out vec3 color;

void main(){
	if(Empty == 1)
		color = vec3(0.5f, 0.5f, 0.5f);
	else if(ht == 0)   
		color = vec3(0, 0, 1); //blue
	else if(ht < 0)
		color = vec3(0, 0.1f + ht/10, 0); //->green
	else if (ht < 500)   
		color = vec3(0, 0.1f + ht/500 * 0.9f, 0); //->green
	else if (ht < 1000)  
		color = vec3(ht/500-1, 1, 0); //->yellow
	else if (ht < 2000)  
		color = vec3(1, 2-ht/1000, 0); //->red
	else                 
		color = vec3(1, ht/2000-1, ht/2000-1);  //->white
}