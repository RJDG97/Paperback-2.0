#version 450 core

layout (location=0) out vec4 fFragClr;
layout (location=1) out vec4 fBrightClr;

void main()
{
	fFragClr = vec4(1.0, 0.0, 0.0, 1.0);
	fBrightClr = vec4(0.0, 0.0, 0.0, 1.0);
}