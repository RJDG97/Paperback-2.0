#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=5) in ivec4 vVertexBoneIDs; 
layout (location=6) in vec4 vVertexWeights;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModel;

uniform mat4 uFinalBonesMatrices[100];
uniform bool uHasBones;

void main()
{
	vec4 TransformedPosition;

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

		TransformedPosition = uModel * BoneTransform * vec4(vVertexPosition, 1.0f);
	}
	else
	{
		TransformedPosition = uModel * vec4(vVertexPosition, 1.0);
	}

	gl_Position = uLightSpaceMatrix * TransformedPosition;
}