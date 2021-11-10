#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec3 vVertexNormal;
layout (location=2) in vec2 vVertexUV;

layout (location=0) out vec2 vUV;

uniform mat4 uModel;

void main()
{
	vUV = vVertexUV;
	gl_Position = uModel * vec4(vVertexPosition, 1.0);
}