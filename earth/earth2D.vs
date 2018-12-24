#version 330 core

uniform vec2 Offset;
uniform vec2 Translate;
uniform vec2 Scale;

layout(location = 0) in vec3 vertexPos;

out vec3 vsColor;

void main(){
	vec2 pos2d = (vec2(vertexPos.x, vertexPos.z) + Offset + Translate) * Scale;// + Translate;
	gl_Position =  vec4(pos2d.x, pos2d.y, 0, 1);

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