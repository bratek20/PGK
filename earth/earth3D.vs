#version 330 core

uniform vec2 Offset;
uniform vec2 Translate;
uniform float Scale;
uniform float Ratio;
uniform int Marked;

layout(location = 0) in float height;

out vec3 vsColor;

void main(){
	float lon = Offset.x + (gl_VertexID % 1201) / 1201.0f;
    float lat = Offset.y + (1201 - gl_VertexID / 1201) / 1201.0f;
	
	r = 6371000 + height
	x = r*cos(lat)*cos(lon)
	y = r*cos(lat)*sin(lon)
	z = r*sin(lat)

	vec2 pos3d = vec3(x, y, z);
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