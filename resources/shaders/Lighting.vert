#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec3 vVertexNormal;
layout (location=2) in vec2 vVertexUV;
layout (location=3) in vec3 vVertexTangent;
layout (location=4) in vec3 vVertextBiTangent;

layout (location=0) out vec3 vPosition;
layout (location=1) out vec3 vNormal;
layout (location=2) out vec2 vUV;

layout (location=3) out vec3 tLightPosition;
layout (location=4) out vec3 tViewPosition;
layout (location=5) out vec3 tFragPosition;

struct Light
{
	vec3 Direction;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform Light uLight;
uniform vec3 uCameraPosition;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	vPosition = vec3(uModel * vec4(vVertexPosition, 1.0));
	vNormal = mat3(transpose(inverse(uModel))) * vVertexNormal;
	vUV = vVertexUV;

	// Tangent space calculations
	mat3 NormalMat = transpose(inverse(mat3(uModel)));
	vec3 T = normalize(NormalMat * vVertexTangent);
	vec3 N = normalize(NormalMat * vVertexNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));
	tLightPosition = TBN * (vPosition - uLight.Direction);
	tViewPosition = TBN * uCameraPosition;
	tFragPosition = TBN * vPosition;

	gl_Position = uProjection * uView * vec4(vPosition, 1.0);
}