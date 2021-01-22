#version 410

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

uniform vec3 LightPos;
uniform float shininess;

out vec4 frag_color;


void main() {
	
	//Lecture 05
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor * inColor;

	//Diffuse
	vec3 N = normalize(inNormal);
	vec3 LightDir = normalize(LightPos - inPos);

	float diff = max(dot(N, LightDir), 0.0);
	vec3 diffuse = diff * inColor * 4.0; // add diffuse intensity

	// Attenuation
	float dist = length(LightPos - inPos);
	diffuse = diffuse / dist; //(dist*dist)

	/**/
	// Specular
	vec3 camPos = vec3(0.0, 0.0, 3.0) ; //Pass this as a uniform from your c++ code
	float specularStrength = 1.0; // this can be a uniform
	vec3 camDir = normalize(camPos - inPos);
	vec3 reflectDir = reflect(-LightDir, N);
	float spec = pow(max(dot(camDir, reflectDir), 0.0), 128); // shininess coefficient (can be uniform)
//	vec3 specular = specularStrength * spec * lightColor; // can also use a specular color

	camDir = normalize(camPos - inPos);
	reflectDir = reflect(-LightDir, N);
	spec = pow(max(dot(camDir, reflectDir), 0.0), shininess); // Shininess coefficient (can be a uniform)

	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) + inColor;

	frag_color = vec4(result, 1.0);
}