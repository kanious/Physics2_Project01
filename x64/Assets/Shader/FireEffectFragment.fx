#version 460

in vec4 fColour;
in vec4 fNormal;
in vec4 fVtxWorldPos;
in vec4 fTexUV;
in vec4 fTangent;
in vec4 fBinormal;
uniform sampler2D diffTexture;
uniform int iFrameIndex;

out vec4 daColor;
void main()
{
	vec2 texUV = vec2(0.0f, 0.0f);
	texUV.x = fTexUV.x * 0.25 + ((16 - iFrameIndex) * 0.25f);
	texUV.y = fTexUV.y * 0.25 + ((16 - iFrameIndex) * 0.25f);

	vec4 diffColour = texture(diffTexture, texUV.xy);

	daColor.rgb = diffColour.rgb;
	daColor.a = diffColour.r;
}