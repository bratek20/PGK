#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;

uniform mat3 worldMat;

out vec3 vsColor;

void main(){
	vec3 worldPos = worldMat * vec3(pos, 1);
	gl_Position =  vec4(worldPos, 1);
	vsColor = color;
}

