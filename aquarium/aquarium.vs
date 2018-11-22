#version 330 core

layout(location = 0) in vec3 pos; 

uniform mat4 worldMat;
uniform vec3 color;

out vec3 vsColor;

void main(){
	vec4 worldPos = worldMat * vec4(pos, 1);
	gl_Position =  worldPos;
	vsColor = color;
}

