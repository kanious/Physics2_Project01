#version 460

uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;
uniform vec4 vColor;

in layout(location = 0) vec4 vColour;
in layout(location = 1) vec4 vPos;
in layout(location = 2) vec4 vNormal;
in layout(location = 3) vec4 vTexUV;
in layout(location = 4) vec4 vTangent;
in layout(location = 5) vec4 vBinormal;
in layout(location = 6) vec4 vBoneID;
in layout(location = 7) vec4 vBoneWeight;

out vec4 fColour;

void main()
{
	gl_Position = matProj * matView * matWorld * vPos;

	//fColour = vec4((vColor).xyz, 1.0f);
	fColour = vec4(vColor.x, vColor.y, vColor.z, vColor.w);
}