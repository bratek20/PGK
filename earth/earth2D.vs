#version 330 core

uniform vec2 Offset;
uniform vec2 Translate;
uniform float Scale;
uniform float Ratio;
uniform int Marked;

layout(location = 0) in float height;

out float ht;

void main(){
	float x = (gl_VertexID % 1201) / 1200.0f;
    float z = (1201 - gl_VertexID / 1201) / 1200.0f;
	vec2 pos2d = (vec2(x, z) + Offset) * Scale + Translate;
	gl_Position =  vec4(pos2d.x * Ratio, pos2d.y, 0, 1);

	ht = height;
}