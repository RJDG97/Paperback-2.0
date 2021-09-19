/**********************************************************************************
* \file MeshBuilder.h
* \brief This is the mesh builder class which provides helper function to generate a
* quad mesh used for 2D rendering.
*
* \author  Malcolm Lim 100% Code Contribution
*
*		copyright Copyright (c) 2021 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#include <string>
#include "glm/inc/glm.hpp"
#include <assimp/scene.h>

#include "Model.h"

struct Vertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_UV;
};

class MeshBuilder
{
public:
	MeshBuilder() = default;
	MeshBuilder(const MeshBuilder&) = delete;
	~MeshBuilder() = default;

	MeshBuilder& operator=(const MeshBuilder&) = delete;

	// Create unit squre quad
	static Model Build2DMesh();
	static Model Build3DMesh(const std::string& File);

	// Create quad to fit NDC
	static Model BuildScreenMesh();

private:
	static Model::SubMesh ProcessSubMesh(aiMesh* SubMesh);
	static void ProcessNode(aiNode* Node, const aiScene* Scene, Model& Mesh);
};

#endif