#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uFinal;

void main()
{
	fFragClr = texture(uFinal, vUV);
}