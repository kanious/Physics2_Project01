#version 460

in vec3 Position;
in vec4 fColour;
in vec4 fNormal;
in vec4 fVtxWorldPos;

uniform vec4 eyeLocation;
uniform samplerCube skybox;

out vec4 daColor;
void main()
{
	vec3 I = normalize(Position - eyeLocation.xyz);
	vec3 R = reflect(I, fNormal.xyz);
	vec4 reflectColour = vec4(texture(skybox, R).rgb, 1.0f);
	
	vec4 diffColour = fColour * 0.3f;
	diffColour += reflectColour * 0.90f;

	daColor = vec4(diffColour.xyz, 0.80f);
}