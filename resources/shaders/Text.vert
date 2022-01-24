#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec3 vVertexNormal;
layout (location=2) in vec2 vVertexUV;

layout (location=0) out vec2 vUV;

uniform float uImageSize;
uniform vec2 uAtlasPosition;
uniform vec2 uAtlasSize;
uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	vUV = (vVertexUV * uAtlasSize + uAtlasPosition) / uImageSize;
	gl_Position = uProjection * uView * uTransform * vec4(vVertexPosition, 1.0);
}