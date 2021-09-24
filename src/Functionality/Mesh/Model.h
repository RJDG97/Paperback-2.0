#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "glew/inc/glew.h"

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
	void SetPrimitive(const int& Primitive);
	int GetPrimitive();
	std::vector<SubMesh>& GetSubMeshes();

private:
	std::vector<SubMesh> m_SubMesh;
	int m_Type;
};

#endif