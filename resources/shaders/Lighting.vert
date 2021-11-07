#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec3 vVertexNormal;
layout (location=2) in vec2 vVertexUV;
layout (location=3) in vec3 vVertexTangent;
layout (location=4) in vec3 vVertextBiTangent;
layout (location=5) in ivec4 vVertexBoneIDs; 
layout (location=6) in vec4 vVertexWeights;

layout (location=0) out vec3 vPosition;
layout (location=1) out vec3 vNormal;
layout (location=2) out vec2 vUV;

layout (location=3) out vec3 tLightPosition;
layout (location=4) out vec3 tViewPosition;
layout (location=5) out vec3 tFragPosition;

layout (location=6) out vec4 lFragPosition;

struct Light
{
	vec3 Direction;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	mat4 Transform;
};

uniform Light uLight;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uCameraPosition;

uniform mat4 uFinalBonesMatrices[100];
uniform mat4 uParentSocketTransform;
uniform bool uHasBones;
uniform bool uHasSocketed;

void main()
{
	vec4 TransformedPosition;
	mat4 CombinedTransform = mat4(1.0f);

	if (uHasBones == true)
	{
		mat4 BoneTransform = mat4(0.0f);

		for (int i = 0 ; i < 4 ; ++i)
		{
			int Id = vVertexBoneIDs[i];

			if (Id != -1)
			{
				BoneTransform += uFinalBonesMatrices[Id] * vVertexWeights[i];
			}
		}

		CombinedTransform *= BoneTransform;
	}

	if (uHasSocketed == true)
	{
		CombinedTransform *= uParentSocketTransform;
	}

	TransformedPosition = uModel * CombinedTransform * vec4(vVertexPosition, 1.0f);
	vPosition = vec3(TransformedPosition);
	vNormal = mat3(transpose(inverse(uModel))) * mat3(CombinedTransform) * vVertexNormal;
	vUV = vVertexUV;

	// Tangent space calculations
	mat3 NormalMat = transpose(inverse(mat3(uModel)));
	vec3 T = normalize(NormalMat * vVertexTangent);
	vec3 N = normalize(NormalMat * vVertexNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	// Tangent space transform
	mat3 TBN = transpose(mat3(T, B, N));
	tLightPosition = TBN * (vPosition - uLight.Direction);
	tViewPosition = TBN * uCameraPosition;
	tFragPosition = TBN * vPosition;
	
	// Light space transform
	lFragPosition = uLight.Transform * vec4(vPosition, 1.0);
	gl_Position = uProjection * uView * TransformedPosition;
}