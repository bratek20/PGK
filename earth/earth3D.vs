#version 330 core

uniform vec2 Offset;
uniform float Ratio;
uniform mat4 VP;
uniform float Radius;
uniform float HeightScale;

layout(location = 0) in float height;

out float ht;

void main(){
	float lon = Offset.x + (gl_VertexID % 1201) / 1200.0f;
    float lat = Offset.y + (1201 - gl_VertexID / 1201) / 1200.0f;
	lon = radians(lon);
	lat = radians(lat);
	
	float r = Radius + HeightScale * height / 1000;
	float x = r*cos(lat)*cos(lon);
	float y = r*cos(lat)*sin(lon);
	float z = r*sin(lat);

	vec3 pos3d = vec3(x, y, z);
	gl_Position =  VP * vec4(pos3d, 1);
	ht = height;
}