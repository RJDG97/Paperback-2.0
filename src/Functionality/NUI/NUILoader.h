#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "../Mesh/Model.h"

class NUILoader
{
public:

	struct TempNodeData
	{
		glm::mat4 transformation;
		std::string name;
		std::string parent_name;
		std::vector<TempNodeData> children;
	};

	struct TempMaterial
	{
		std::pair<std::string, std::string> m_Ambient; //texture name, path
		std::pair<std::string, std::string> m_Diffuse;
		std::pair<std::string, std::string> m_Specular;
		std::pair<std::string, std::string> m_Normal;
	};

	Model LoadNui(std::string file_path);

	void SetUpNodeData(NodeData& node, std::vector<TempNodeData>& node_datas);

	Model::SubMesh CreateSubMesh(std::vector<Model::Vertex> vertices, std::vector<GLushort> indices, std::pair<std::string, TempMaterial> material);
};