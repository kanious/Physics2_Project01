#version 460

uniform mat4 matWorld;
uniform mat4 matView;
uniform mat4 matProj;

in layout(location = 0) vec4 vColour;
in layout(location = 1) vec4 vPos;
in layout(location = 2) vec4 vNormal;
in layout(location = 3) vec4 vTexUV;
in layout(location = 4) vec4 vTangent;
in layout(location = 5) vec4 vBinormal;
in layout(location = 6) vec4 vBoneID;
in layout(location = 7) vec4 vBoneWeight;

out vec4 fColour;
out vec4 fNormal;
out vec4 fVtxWorldPos;
out vec4 fTexUV;
out vec4 fTangent;
out vec4 fBinormal;

void main()
{
	gl_Position = matProj * matView * matWorld * vPos;

	fVtxWorldPos = vec4((matWorld * vPos).xyz, 1.0f);
	fNormal = vec4(normalize(transpose(inverse(matWorld)) * vNormal).xyz, 1.0f);

	fColour = vColour;
	fTexUV = vTexUV;
	fTangent = vTangent;
	fBinormal = vBinormal;
}