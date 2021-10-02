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

#include "Functionality/RenderResource/RenderResourceManager.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <vector>

Model MeshBuilder::Build2DMesh()
{
	// Set vertex position
	glm::vec3 p0{ -0.5f, 0.5f, 0.f };
	glm::vec3 p1{ -0.5f, -0.5f, 0.f };
	glm::vec3 p2{ 0.5f, -0.5f, 0.f };
	glm::vec3 p3{ 0.5f, 0.5f, 0.f };

	// Set vertex normal
	glm::vec3 normal = glm::vec3{ 0.f, 0.f, 1.f };

	// Set vertex uv
	glm::vec2 uv0{ 0.f, 1.f };
	glm::vec2 uv1{ 0.f, 0.f };
	glm::vec2 uv2{ 1.f, 0.f };
	glm::vec2 uv3{ 1.f, 1.f };

	glm::vec3 tangent0, bitangent0, tangent1, bitangent1;

	glm::vec3 edge0 = p1 - p0;
	glm::vec3 edge1 = p2 - p0;
	glm::vec2 deltaUV0 = uv1 - uv0;
	glm::vec2 deltaUV1 = uv2 - uv0;

	float f = 1.f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

	tangent0.x = f * (deltaUV1.y * edge0.x - deltaUV0.y * edge1.x);
	tangent0.y = f * (deltaUV1.y * edge0.y - deltaUV0.y * edge1.y);
	tangent0.z = f * (deltaUV1.y * edge0.z - deltaUV0.y * edge1.z);

	bitangent0.x = f * (-deltaUV1.x * edge0.x + deltaUV0.x * edge1.x);
	bitangent0.y = f * (-deltaUV1.x * edge0.y + deltaUV0.x * edge1.y);
	bitangent0.z = f * (-deltaUV1.x * edge0.z + deltaUV0.x * edge1.z);

	edge0 = p2 - p0;
	edge1 = p3 - p0;
	deltaUV0 = uv2 - uv0;
	deltaUV1 = uv3 - uv0;

	f = 1.f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

	tangent1.x = f * (deltaUV1.y * edge0.x - deltaUV0.y * edge1.x);
	tangent1.y = f * (deltaUV1.y * edge0.y - deltaUV0.y * edge1.y);
	tangent1.z = f * (deltaUV1.y * edge0.z - deltaUV0.y * edge1.z);

	bitangent1.x = f * (-deltaUV1.x * edge0.x + deltaUV0.x * edge1.x);
	bitangent1.y = f * (-deltaUV1.x * edge0.y + deltaUV0.x * edge1.y);
	bitangent1.z = f * (-deltaUV1.x * edge0.z + deltaUV0.x * edge1.z);

	std::vector<Model::Vertex> vertices;

	vertices.push_back({ p0, normal, uv0, tangent0, bitangent0 });
	vertices.push_back({ p1, normal, uv1, tangent0, bitangent0 });
	vertices.push_back({ p2, normal, uv2, tangent0, bitangent0 });
	vertices.push_back({ p0, normal, uv0, tangent1, bitangent1 });
	vertices.push_back({ p2, normal, uv2, tangent1, bitangent1 });
	vertices.push_back({ p3, normal, uv3, tangent1, bitangent1 });

	// Create a handle for vbo
	GLuint vbo;
	glCreateBuffers(1, &vbo);

	// Set size of vbo
	glNamedBufferStorage(vbo, sizeof(Model::Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	// Create the array to store vertice indexes
	std::vector<GLushort> index = { 0,1,2,3,4,5 };

	// Create a handle for ebo
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(GLushort) * index.size(), reinterpret_cast<GLvoid*>(index.data()), GL_DYNAMIC_STORAGE_BIT);

	Model model;

	model.AddSubMesh(Model::SubMesh{ vertices, index, vbo, ebo, static_cast<GLuint>(index.size()) });
	model.SetPrimitive(GL_TRIANGLES);

	return std::move(model);
}

Model MeshBuilder::Build3DMesh(const std::string& File)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate |
												   aiProcess_LimitBoneWeights |
												   aiProcess_FindInstances |
												   aiProcess_GenSmoothNormals |
												   aiProcess_CalcTangentSpace |
												   aiProcess_JoinIdenticalVertices |
												   aiProcess_RemoveRedundantMaterials |
												   aiProcess_FindInvalidData);

	Model model;

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return model;

	ProcessNode(scene->mRootNode, scene, model);

	model.SetPrimitive(GL_TRIANGLES);

	importer.FreeScene();

	return std::move(model);
}

void MeshBuilder::ProcessNode(aiNode* Node, const aiScene* Scene, Model& Mesh)
{
	for (size_t i = 0; i < Node->mNumMeshes; ++i)
	{
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		Mesh.AddSubMesh(ProcessSubMesh(mesh, Scene));
	}

	for (size_t i = 0; i < Node->mNumChildren; ++i)
	{
		ProcessNode(Node->mChildren[i], Scene, Mesh);
	}
}

Model::SubMesh MeshBuilder::ProcessSubMesh(aiMesh* SubMesh, const aiScene* Scene)
{
	std::vector<Model::Vertex> vertices;
	std::vector<GLushort> index;

	for (size_t i = 0; i < SubMesh->mNumVertices; ++i)
	{
		glm::vec3 position{ 0,0,0 }, normal{ 0,0,0 }, tangent{ 0,0,0 }, bitangent{ 0,0,0 };
		glm::vec2 uv{ 0,0 };

		position = glm::vec3{ SubMesh->mVertices[i].x, SubMesh->mVertices[i].y, SubMesh->mVertices[i].z };

		if (SubMesh->HasNormals())
			normal = glm::vec3{ SubMesh->mNormals[i].x, SubMesh->mNormals[i].y ,SubMesh->mNormals[i].z };

		if (SubMesh->mTextureCoords[0])
		{
			uv = glm::vec2{ SubMesh->mTextureCoords[0][i].x, SubMesh->mTextureCoords[0][i].y };
			tangent = glm::vec3{ SubMesh->mTangents[i].x, SubMesh->mTangents[i].y, SubMesh->mTangents[i].z };
			bitangent = glm::vec3{ SubMesh->mBitangents[i].x, SubMesh->mBitangents[i].y, SubMesh->mBitangents[i].z };
		}

		vertices.push_back({ position, normal, uv, tangent, bitangent });
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
	glNamedBufferStorage(vbo, sizeof(Model::Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	// Create a handle for ebo
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(GLushort) * index.size(), reinterpret_cast<GLvoid*>(index.data()), GL_DYNAMIC_STORAGE_BIT);

	aiString str;
	aiMaterial* material = Scene->mMaterials[SubMesh->mMaterialIndex];
	material->Get(AI_MATKEY_NAME, str);

	std::string mat = RenderResourceManager::GetInstanced().LoadMaterial(str.C_Str(), material);

	return std::move(Model::SubMesh{ vertices, index, vbo, ebo, static_cast<GLuint>(index.size()), mat });
}

Model MeshBuilder::BuildScreenMesh()
{
	std::vector<Model::Vertex> vertices;
	vertices.resize(4);

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
	glNamedBufferStorage(vbo, sizeof(Model::Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	// Create the array to store vertice indexes
	std::vector<GLushort> index = { 0, 1, 2, 3 };

	// Create a handle for ebo
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(GLushort) * index.size(), reinterpret_cast<GLvoid*>(index.data()), GL_DYNAMIC_STORAGE_BIT);

	Model model;

	model.AddSubMesh(Model::SubMesh{ vertices, index, vbo, ebo, static_cast<GLuint>(index.size()) });
	model.SetPrimitive(GL_TRIANGLE_STRIP);

	return std::move(model);
}