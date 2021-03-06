#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTranslation_instanced;
layout(location = 4) in vec3 vertexScale_instanced;
layout(location = 5) in vec3 vertexColor_instanced;

out vec3 Position_modelspace;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec2 UV;
out vec3 VertexColor;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace[10];

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform int LightsNum;
uniform vec3 LightPosition_worldspace[10];
uniform int ReverseNormal;
uniform int IsInstanced;
uniform vec3 MeshColor;

void main(){
	vec3 vertexPosition = vertexPosition_modelspace;
	VertexColor = MeshColor;
	if(IsInstanced == 1){
		vertexPosition = vertexPosition * vertexScale_instanced + vertexTranslation_instanced;
		VertexColor = vertexColor_instanced;
	}

	gl_Position =  MVP * vec4(vertexPosition, 1);
	// Position of the vertex, in worldspace : M * position
	Position_modelspace = vertexPosition_modelspace;
	Position_worldspace = (M * vec4(vertexPosition,1)).xyz;
	
	vec3 normal_modelspace = vertexNormal_modelspace;
	if(ReverseNormal != 0){
		normal_modelspace = -normal_modelspace;
	}
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	for(int i=0;i<LightsNum;i++){
		vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace[i],1)).xyz;
		LightDirection_cameraspace[i] = LightPosition_cameraspace + EyeDirection_cameraspace;
	}

	// Normal of the the vertex, in camera space  
	Normal_cameraspace = mat3(V) * mat3(transpose(inverse(M))) * normal_modelspace;

	UV = vertexUV;
}