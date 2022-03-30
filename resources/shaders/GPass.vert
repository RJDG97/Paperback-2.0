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
layout (location=2) out vec3 vTangent;
layout (location=3) out vec3 vBiTangent;
layout (location=4) out vec2 vUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uCameraPosition;

uniform mat4 uFinalBonesMatrices[100];
uniform bool uHasBones;

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
	
	mat4 FinalTransform = uView * uModel * CombinedTransform;

	TransformedPosition = FinalTransform * vec4(vVertexPosition, 1.0f);

	// Vertex transform
	vPosition = vec3(TransformedPosition);
	vUV = vVertexUV;

	// Convert TBN to world space
	mat3 NormalMat = transpose(inverse(mat3(FinalTransform)));
	vec3 T = normalize(NormalMat * vVertexTangent);
	vec3 N = normalize(NormalMat * vVertexNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	vTangent = T;
	vBiTangent = B;
	vNormal = N;

	gl_Position = uProjection * TransformedPosition;
}