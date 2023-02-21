#version 460

in vec4 fColour;
in vec4 fNormal;
in vec4 fVtxWorldPos;
in vec4 fTexUV;
in vec4 fTangent;
in vec4 fBinormal;
uniform sampler2D diffTexture;


out vec4 daColor;
void main()
{
	vec4 diffColour = texture(diffTexture, fTexUV.xy);
	diffColour *= 1.35f;

	daColor = vec4(diffColour.xyz, 1.0f);
}