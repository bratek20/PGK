#version 330 core

uniform vec2 Offset;
uniform float Ratio;
uniform mat4 VP;
uniform float Radius;
uniform int Empty;

layout(location = 0) in float height;

out vec3 vsColor;

void main(){
	float lon = Offset.x + (gl_VertexID % 1201) / 1200.0f;
    float lat = Offset.y + (1201 - gl_VertexID / 1201) / 1200.0f;
	lon = radians(lon);
	lat = radians(lat);
	
	float r = Radius + height / 100;
	float x = r*cos(lat)*cos(lon);
	float y = r*cos(lat)*sin(lon);
	float z = r*sin(lat);

	vec3 pos3d = vec3(x, y, z);
	gl_Position =  VP * vec4(pos3d, 1);

	float ht = height;
	if(Empty == 1)
		vsColor = vec3(0.5f, 0.5f, 0.5f);
	else if(ht <= 0)   
		vsColor = vec3(0, 0, 1 + ht/10); //blue
	else if (ht < 500)   
		vsColor = vec3(0, 0.25f + ht/500 * 0.75f, 0); //->green
	else if (ht < 1000)  
		vsColor = vec3(ht/500-1, 1, 0); //->yellow
	else if (ht < 2000)  
		vsColor = vec3(1, 2-ht/1000, 0); //->red
	else                 
		vsColor = vec3(1, ht/2000-1, ht/2000-1);  //->white
}