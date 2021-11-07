#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=5) in ivec4 vVertexBoneIDs; 
layout (location=6) in vec4 vVertexWeights;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModel;

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
	gl_Position = uLightSpaceMatrix * TransformedPosition;
}