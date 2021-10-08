#version 450 core

layout (location=0) out vec4 fFragClr;
layout (location=1) out vec4 fBrightClr;

uniform bool uRed;

void main()
{
	if(uRed)
		fFragClr = vec4(1.0, 0.0, 0.0, 1.0);
	else
		fFragClr = vec4(0.0, 1.0, 0.0, 1.0);
	
	fBrightClr = vec4(0.0, 0.0, 0.0, 1.0);
}