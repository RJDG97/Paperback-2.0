#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec3 vVertexNormal;
layout (location=2) in vec2 vVertexUV;
layout (location=3) in vec3 vVertexTangent;
layout (location=4) in vec3 vVertextBiTangent;
layout (location=5) in ivec4 vVertexBoneIDs; 
layout (location=6) in vec4 vVertexWeights;
layout (location=7) in mat4 iTransform;
layout (location=11) in float iOpacity;

layout (location=0) out vec2 vUV;
layout (location=1) out float vOpacity;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	vUV = vVertexUV;
	vOpacity = iOpacity;

	gl_Position = uProjection * uView * iTransform * vec4(vVertexPosition, 1.0);
}