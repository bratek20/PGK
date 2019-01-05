#version 330 core

uniform vec2 Offset;
uniform vec2 Translate;
uniform float Scale;
uniform float Ratio;
uniform int Marked;

layout(location = 0) in float height;

out vec3 vsColor;

void main(){
	float x = (gl_VertexID % 1201) / 1200.0f;
    float z = (1201 - gl_VertexID / 1201) / 1200.0f;
	vec2 pos2d = (vec2(x, z) + Offset) * Scale + Translate;
	gl_Position =  vec4(pos2d.x * Ratio, pos2d.y, 0, 1);

	float ht = height;
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

	if(Marked == 1)
		vsColor = vec3(1,0,0);
}