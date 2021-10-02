#version 450 core

layout (location=0) in vec3 vVertexPosition;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModel;

void main()
{
	gl_Position = uLightSpaceMatrix * uModel * vec4(vVertexPosition, 1.0);
}