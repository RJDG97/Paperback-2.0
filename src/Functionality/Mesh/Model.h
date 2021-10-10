#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <unordered_map>
#include "glew/inc/glew.h"
#include "../Animation/Animation.h"

class Model
{
public:
	struct Vertex
	{
		glm::vec3 m_Position;
		glm::vec3 m_Normal;
		glm::vec2 m_UV;
		glm::vec3 m_Tangent;
		glm::vec3 m_BiTangent;

		int m_BoneIDs[4];
		float m_Weights[4];

		Vertex() = default;

		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 tangent, glm::vec3 bitangent, int* bone_ids = {}, float* weights = {})
			: m_Position{ position },
			  m_Normal { normal },
			  m_UV { uv },
			  m_Tangent { tangent },
			  m_BiTangent { bitangent }

		{
			if (bone_ids)
			{
				memcpy(m_BoneIDs, bone_ids, 4 * sizeof(int));
			}

			if (weights)
			{
				memcpy(m_Weights, weights, 4 * sizeof(int));
			}
		}
	};

	struct Material
	{
		std::string m_Ambient;
		std::string m_Diffuse;
		std::string m_Specular;
		std::string m_Normal;
	};

	struct SubMesh
	{
		std::vector<Vertex> m_Vertices;
		std::vector<GLushort> m_Indices;

		// Vertex buffer
		GLuint m_VBO;
		// Index buffer
		GLuint m_EBO;
		// Draw count
		GLuint m_DrawCount;

		// Submesh material
		std::string m_Material;
	};

	Model() = default;
	~Model();

	void AddSubMesh(const SubMesh& Mesh);
	void RemoveAllSubMesh();
	void AddAnimation(const Animation& animation, std::string animation_name);
	std::unordered_map<std::string, Animation>& GetAnimations() { return m_Animations; }
	void SetPrimitive(const int& Primitive);
	int GetPrimitive() const;
	const std::vector<SubMesh>& GetSubMeshes() const;

	auto& GetBoneInfoMap() { return m_BoneInfoMap; }

private:
	std::vector<SubMesh> m_SubMesh;
	int m_Type;
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
	std::unordered_map<std::string, Animation> m_Animations;
};

#endif