#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;

uniform bool uTexturedDiffuse;
uniform bool uTexturedAmbient;
uniform bool uTexturedSpecular;
uniform bool uTexturedNormal;

uniform sampler2D uDiffuse;
uniform sampler2D uAmbient;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;

void main()
{
	// Write to color texture
	if(uTexturedDiffuse)
	{
		fFragClr = texture(uDiffuse, vUV);
	}
	else
	{
		fFragClr = vec4(1.0, 0.0, 0.5, 1.0);
	}
}