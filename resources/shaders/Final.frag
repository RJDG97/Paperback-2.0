#version 450 core

layout (location=0) in vec2 vUV;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uFinal;

void main()
{
	vec3 Color = texture(uFinal, vUV).rgb;
	fFragClr = vec4(Color, 1.0);
}