#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "glew/inc/glew.h"

class Model
{
public:
	struct SubMesh
	{
		// Vertex buffer
		GLuint m_VBO;
		// Index buffer
		GLuint m_EBO;
		// Draw count
		GLuint m_DrawCount;
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