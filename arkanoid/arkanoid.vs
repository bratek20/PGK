#version 330 core

layout(location = 0) in vec2 pos;

uniform vec2 translation;
uniform vec2 scale;
uniform vec3 color;
uniform float time;
uniform float timeScaleImpact;
out vec3 vsColor;

void main(){
	float timeScaleRange = (sin(time) + 1)/2; // [0, 1]
	float timeScale = 1 + timeScaleImpact * timeScaleRange;
	vec2 scaledPos = vec2(pos.x * scale.x, pos.y * scale.y) * timeScale; 
	gl_Position =  vec4(scaledPos + translation, 0, 1);
	vsColor = color * timeScale;
}

