#version 450 core

layout (location=0) in vec3 vVertexPosition;

uniform mat4 uTransform;

void main()
{
	gl_Position = uTransform * vec4(vVertexPosition, 1.0);
}