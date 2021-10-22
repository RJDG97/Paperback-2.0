#version 450 core

layout (location=0) in vec3 vUV;

layout (location=0) out vec4 fFragClr;
layout (location=1) out vec4 fBrightClr;

uniform samplerCube uSkybox;

void main()
{
	fFragClr = texture(uSkybox, vUV);
	fBrightClr = vec4(0.0, 0.0, 0.0, 1.0);
}