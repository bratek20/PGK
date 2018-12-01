#version 330 core

in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[10];

// Ouput data
out vec3 color;

uniform vec3 MeshColor;
uniform int LightsNum;
uniform vec3 LightPosition_worldspace[10];
uniform vec3 LightColor[10];
uniform float LightPower[10];
uniform vec3 LightDistanceCoefficients[10];

void main(){
	// Material properties
	vec3 MaterialDiffuseColor = MeshColor;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	color = vec3(0,0,0);
	for(int i = 0; i < LightsNum; i++){
		// Distance to the light
		float distance = length( LightPosition_worldspace[i] - Position_worldspace );

		// Normal of the computed fragment, in camera space
		vec3 n = normalize( Normal_cameraspace );
		// Direction of the light (from the fragment to the light)
		vec3 l = normalize( LightDirection_cameraspace[i] );
		// Cosine of the angle between the normal and the light direction, 
		// clamped above 0
		//  - light is at the vertical of the triangle -> 1
		//  - light is perpendicular to the triangle -> 0
		//  - light is behind the triangle -> 0
		float cosTheta = clamp( dot( n,l ), 0,1 );
		
		// Eye vector (towards the camera)
		vec3 E = normalize(EyeDirection_cameraspace);
		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l,n);
		// Cosine of the angle between the Eye vector and the Reflect vector,
		// clamped to 0
		//  - Looking into the reflection -> 1
		//  - Looking elsewhere -> < 1
		float cosAlpha = clamp( dot( E,R ), 0,1 ); 

		float distanceLoss = 
			distance * distance * LightDistanceCoefficients[i].x +
			distance * LightDistanceCoefficients[i].y +
			LightDistanceCoefficients[i].z;  

		color += 
			// Ambient : simulates indirect lighting
			MaterialAmbientColor
			// Diffuse : "color" of the object
			+ MaterialDiffuseColor * LightColor[i] * LightPower[i] * cosTheta / distanceLoss
			// Specular : reflective highlight, like a mirror
			+ MaterialSpecularColor * LightColor[i] * LightPower[i] * pow(cosAlpha,5) / distanceLoss;
	}
}