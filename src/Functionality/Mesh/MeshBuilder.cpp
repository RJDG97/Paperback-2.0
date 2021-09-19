/**********************************************************************************
* \file MeshBuilder.cpp
* \brief This is the mesh builder class which provides helper function to generate a
* quad mesh used for 2D rendering.
*
* \author  Malcolm Lim 100% Code Contribution
*
*		copyright Copyright (c) 2021 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#include "MeshBuilder.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
//#include "assimp/inc/Importer.hpp"
//#include "assimp/inc/postprocess.h"

#include <array>
#include <vector>

Model MeshBuilder::Build2DMesh()
{
	std::array<Vertex, 4> vertices;

	// Set vertex position
	vertices[0].m_Position = glm::vec3(0.5f, -0.5f, 0.f);
	vertices[1].m_Position = glm::vec3(0.5f, 0.5f, 0.f);
	vertices[2].m_Position = glm::vec3(-0.5f, 0.5f, 0.f);
	vertices[3].m_Position = glm::vec3(-0.5f, -0.5f, 0.f);

	// Set vertex normal
	vertices[0].m_Normal = glm::vec3{ 0.f, 0.f, 1.f };
	vertices[1].m_Normal = glm::vec3{ 0.f, 0.f, 1.f };
	vertices[2].m_Normal = glm::vec3{ 0.f, 0.f, 1.f };
	vertices[3].m_Normal = glm::vec3{ 0.f, 0.f, 1.f };

	// Set vertex uv
	vertices[0].m_UV = glm::vec2(1, 1);
	vertices[1].m_UV = glm::vec2(1, 0);
	vertices[2].m_UV = glm::vec2(0, 0);
	vertices[3].m_UV = glm::vec2(0, 1);

	// Create a handle for vbo
	GLuint vbo;
	glCreateBuffers(1, &vbo);

	// Set size of vbo
	glNamedBufferStorage(vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	// Create the array to store vertice indexes
	std::vector<GLushort> index = { 2,3,1,0 };

	// Create a handle for ebo
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(GLushort) * index.size(), reinterpret_cast<GLvoid*>(index.data()), GL_DYNAMIC_STORAGE_BIT);

	Model model;

	model.AddSubMesh(Model::SubMesh{ vbo, ebo, static_cast<GLuint>(index.size()) });
	model.SetPrimitive(GL_TRIANGLE_STRIP);

	return model;
}

Model MeshBuilder::Build3DMesh(const std::string& File)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	Model model;

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return model;

	ProcessNode(scene->mRootNode, scene, model);

	model.SetPrimitive(GL_TRIANGLES);

	importer.FreeScene();

	return model;
}

void MeshBuilder::ProcessNode(aiNode* Node, const aiScene* Scene, Model& Mesh)
{
	for (size_t i = 0; i < Node->mNumMeshes; ++i)
	{
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		Mesh.AddSubMesh(ProcessSubMesh(mesh));
	}

	for (size_t i = 0; i < Node->mNumChildren; ++i)
	{
		ProcessNode(Node->mChildren[i], Scene, Mesh);
	}
}

Model::SubMesh MeshBuilder::ProcessSubMesh(aiMesh* SubMesh)
{
	std::vector<Vertex> vertices;
	std::vector<GLushort> index;

	for (size_t i = 0; i < SubMesh->mNumVertices; ++i)
	{
		glm::vec3 position{ 0,0,0 }, normal{ 0,0,0 };
		glm::vec2 uv{ 0,0 };

		position = glm::vec3{ SubMesh->mVertices[i].x, SubMesh->mVertices[i].y, SubMesh->mVertices[i].z };

		if (SubMesh->HasNormals())
			normal = glm::vec3{ SubMesh->mNormals[i].x, SubMesh->mNormals[i].y ,SubMesh->mNormals[i].z };
		
		if (SubMesh->mTextureCoords[0])
			uv = glm::vec2{ SubMesh->mTextureCoords[0][i].x, SubMesh->mTextureCoords[0][i].y };

		vertices.push_back({ position, normal, uv });
	}

	for (size_t i = 0; i < SubMesh->mNumFaces; ++i)
	{
		aiFace face = SubMesh->mFaces[i];

		for (size_t j = 0; j < face.mNumIndices; ++j)
			index.push_back(face.mIndices[j]);
	}

	// Create a handle for vbo
	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	// Create a handle for ebo
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(GLushort) * index.size(), reinterpret_cast<GLvoid*>(index.data()), GL_DYNAMIC_STORAGE_BIT);

	return Model::SubMesh{ vbo, ebo, static_cast<GLuint>(index.size()) };
}

Model MeshBuilder::BuildScreenMesh()
{
	std::array<Vertex, 4> vertices;

	// Set vertex position
	vertices[0].m_Position = glm::vec3(-1.f, 1.f, 0.f);
	vertices[1].m_Position = glm::vec3(-1.f, -1.f, 0.f);
	vertices[2].m_Position = glm::vec3(1.f, 1.f, 0.f);
	vertices[3].m_Position = glm::vec3(1.f, -1.f, 0.f);

	// Set vertex normal
	vertices[0].m_Normal = glm::vec3{ 0.f, 0.f, 1.f };
	vertices[1].m_Normal = glm::vec3{ 0.f, 0.f, 1.f };
	vertices[2].m_Normal = glm::vec3{ 0.f, 0.f, 1.f };
	vertices[3].m_Normal = glm::vec3{ 0.f, 0.f, 1.f };

	// Set vertex uv
	vertices[0].m_UV = glm::vec2(0.f, 1.f);
	vertices[1].m_UV = glm::vec2(0.f, 0.f);
	vertices[2].m_UV = glm::vec2(1.f, 1.f);
	vertices[3].m_UV = glm::vec2(1.f, 0.f);

	// Create a handle for vbo
	GLuint vbo;
	glCreateBuffers(1, &vbo);

	// Set size of vbo
	glNamedBufferStorage(vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	// Create the array to store vertice indexes
	std::vector<GLushort> index = { 0, 1, 2, 3 };

	// Create a handle for ebo
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(GLushort) * index.size(), reinterpret_cast<GLvoid*>(index.data()), GL_DYNAMIC_STORAGE_BIT);

	Model model;

	model.AddSubMesh(Model::SubMesh{ vbo, ebo, static_cast<GLuint>(index.size()) });
	model.SetPrimitive(GL_TRIANGLE_STRIP);

	return model;
}