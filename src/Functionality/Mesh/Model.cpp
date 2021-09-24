#include "../build/Paperback_V2/paperback_pch.h"
#include "Model.h"

Model::~Model()
{
}

void Model::AddSubMesh(const SubMesh& Mesh)
{
	m_SubMesh.push_back(Mesh);
}

void Model::RemoveAllSubMesh()
{
	for (auto& submesh : m_SubMesh)
	{
		glDeleteBuffers(1, &submesh.m_VBO);
		glDeleteBuffers(1, &submesh.m_EBO);
	}

	m_SubMesh.clear();
}

void Model::SetPrimitive(const int& Primitive)
{
	m_Type = Primitive;
}

int Model::GetPrimitive()
{
	return m_Type;
}

std::vector<Model::SubMesh>& Model::GetSubMeshes()
{
	return m_SubMesh;
}