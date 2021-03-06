#version 330 core

in vec3 Position_modelspace;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec2 UV;
in vec3 VertexColor;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[10];

// Ouput data
out vec3 color;

uniform int LightsNum;
uniform vec3 LightPosition_worldspace[10];
uniform vec3 LightColor[10];
uniform float LightPower[10];
uniform vec3 LightDistanceCoefficients[10];
uniform vec3 PlayerPosition_worldspace;
uniform sampler2D TextureSampler;
uniform sampler2D NoiseTextureSampler;

vec3 noise(vec2 uv){
	return texture( NoiseTextureSampler, UV ).rgb;
}

vec3 genColor(vec2 uv){
	vec3 color = vec3(0, 0, 0);
	for(int i=1;i<=4;i++){
		float pow2 = pow(2,i);
		color += pow(2, -i) * noise(vec2(pow2 * uv.x, pow2 * uv.y));
	}
	return color;
}

void main(){
	// Material properties
	vec3 noiseColor = genColor(UV);
	vec3 textureColor = texture( TextureSampler, UV ).rgb; 
	vec3 MaterialDiffuseColor = 0.33f + VertexColor * textureColor * noiseColor;
	vec3 MaterialAmbientColor = vec3(0.01,0.01,0.01) * MaterialDiffuseColor;
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

	float distToPlayer = length( PlayerPosition_worldspace - Position_worldspace );
	color = color + vec3(0,0, 1 - pow(2.71, -0.01*distToPlayer));
	color = color * pow(2.71, -0.02*distToPlayer);
}