#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=2) in vec2 vVertexUV;

layout (location=0) out vec2 vUV;

void main()
{
	vUV = vVertexUV;
	gl_Position = vec4(vVertexPosition, 1.0);
}