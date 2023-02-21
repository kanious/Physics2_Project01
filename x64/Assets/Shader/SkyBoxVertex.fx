#version 460

in vec3 aPos;

out vec3 TexCoords;

uniform mat4 matView;
uniform mat4 matProj;

void main()
{
	TexCoords = aPos;

	gl_Position = matProj * matView * vec4(aPos, 1.0);
}