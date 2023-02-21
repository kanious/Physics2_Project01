#version 460

in vec4 fColour;
in vec4 fNormal;
in vec4 fVtxWorldPos;

out vec4 daColor;
void main()
{
	daColor = fColour;

	daColor.a = 0.25f;
}