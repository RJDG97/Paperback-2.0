/**********************************************************************************
*\file         Model.cpp
*\brief        Model.cpp
*
*\author	   Malcolm, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

void Model::AddAnimation(const Animation& animation, std::string animation_name)
{
	m_Animations[animation_name] = animation;
}

void Model::SetPrimitive(const int& Primitive)
{
	m_Type = Primitive;
}

int Model::GetPrimitive() const
{
	return m_Type;
}

const std::vector<Model::SubMesh>& Model::GetSubMeshes() const
{
	return m_SubMesh;
}