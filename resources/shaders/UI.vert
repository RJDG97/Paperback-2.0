#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec3 vVertexNormal;
layout (location=2) in vec2 vVertexUV;

layout (location=0) out vec2 vUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	vUV = vVertexUV;
	gl_Position = uProjection * uView * uModel * vec4(vVertexPosition, 1.0);
}