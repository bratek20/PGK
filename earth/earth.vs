#version 330 core

layout(location = 0) in vec3 vertexPos;

out vec3 vsColor;

void main(){
	gl_Position =  vec4(vertexPos.x, vertexPos.z, 0, 1);
	float ht = vertexPos.y;
	if(ht < -32000)
		vsColor = vec3(0, 0, 0);//black, no data
	else if(ht < 0)   
		vsColor = vec3(0, 0, 1); //blue
    else if (ht < 500)   
		vsColor = vec3(0, ht/500, 0); //->green
    else if (ht < 1000)  
		vsColor = vec3(ht/500-1, 1, 0); //->yellow
    else if (ht < 2000)  
		vsColor = vec3(1, 2-ht/1000, 0); //->red
    else                 
		vsColor = vec3(1, ht/2000-1, ht/2000-1);  //->white
}