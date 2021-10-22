#version 450 core

layout (location=0) in vec3 vVertexPosition;

layout (location=0) out vec3 vUV;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	vUV = vVertexPosition;
	vec4 Position = uProjection * uView * vec4(vVertexPosition, 1.0);
	gl_Position = Position.xyww;
}