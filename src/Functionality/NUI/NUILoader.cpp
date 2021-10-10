#include "NUILoader.h"
#include "../RenderResource/RenderResourceManager.h"

Model NUILoader::LoadNui(std::string file_path)
{
	//std::cout << "Loading " << file_path << std::endl;

	Model model;

	std::vector<Model::Vertex> vertices;
	std::vector<GLushort> indices;

	std::ifstream ifs{ file_path };

	if (ifs)
	{
		while (ifs.peek() != EOF)
		{
			std::string next;
			ifs >> next;

			if (next == "VERTEX")
			{
				std::string temp;
				std::getline(ifs, temp);
				std::stringstream vertices_line{ temp };

				glm::vec3 position;
				glm::vec3 normal;
				glm::vec2 uv;
				glm::vec3 tangent;
				glm::vec3 bi_tangent;

				int bone_ids[4];
				float weights[4];

				vertices_line >> position.x >> position.y >> position.z
					>> normal.x >> normal.y >> normal.z
					>> uv.x >> uv.y
					>> tangent.x >> tangent.y >> tangent.z
					>> bi_tangent.x >> bi_tangent.y >> bi_tangent.z
					>> bone_ids[0] >> bone_ids[1] >> bone_ids[2] >> bone_ids[3]
					>> weights[0] >> weights[1] >> weights[2] >> weights[3];

				vertices.push_back({ position, normal, uv, tangent, bi_tangent, bone_ids, weights });
			}

			else if (next == "INDEX")
			{
				std::string temp;
				std::getline(ifs, temp);
				std::stringstream indices_line{ temp };
				GLushort index;

				while (indices_line >> index)
				{
					indices.push_back(index);
				}
			}

			else if (next == "MATERIAL")
			{
				std::string material_name;
				ifs >> material_name;

				TempMaterial temp_material;
				ifs >> temp_material.m_Ambient.first
					>> temp_material.m_Ambient.second;

				ifs >> temp_material.m_Diffuse.first
					>> temp_material.m_Diffuse.second;

				ifs >> temp_material.m_Specular.first
					>> temp_material.m_Specular.second;

				ifs >> temp_material.m_Normal.first
					>> temp_material.m_Normal.second;

				model.AddSubMesh(CreateSubMesh(vertices, indices, { material_name, temp_material }));

				vertices.clear();
				indices.clear();
			}

			else if (next == "BONE")
			{
				auto& bone_info_map{ model.GetBoneInfoMap() };
				std::string name;
				int id;
				float mat_values[16];

				ifs >> name >> id;

				for (int i = 0; i < 16; ++i)
				{
					ifs >> mat_values[i];
				}

				bone_info_map[name] = { id, {mat_values[0],  mat_values[1],  mat_values[2],  mat_values[3],
											 mat_values[4],  mat_values[5],  mat_values[6],  mat_values[7],
											 mat_values[8],  mat_values[9],  mat_values[10], mat_values[11],
											 mat_values[12], mat_values[13], mat_values[14], mat_values[15]} };
			}

			else if (next == "ANIMATION")
			{
				std::string anim_name;
				float duration;
				float ticks_per_second;
				std::vector<Bone> bones;
				NodeData root_node;
				std::unordered_map < std::string, BoneInfo > bone_info_map;

				std::vector<TempNodeData> node_datas;

				ifs >> anim_name >> duration >> ticks_per_second;

				while (ifs.peek() != EOF)
				{
					std::string next_data;
					ifs >> next_data;

					if (next_data == "ANIMATIONBONE")
					{
						std::vector<KeyPosition> positions;
						std::vector<KeyRotation> rotations;
						std::vector<KeyScale> scales;
						glm::mat4 local_transform{};
						std::string name;
						int id{};

						std::string temp;
						std::getline(ifs, temp);
						std::stringstream animationbone_line{ temp };

						while (animationbone_line.rdbuf()->in_avail() != 0)
						{
							std::string next_bone_data;
							animationbone_line >> next_bone_data;

							if (next_bone_data == "POSITION")
							{
								glm::vec3 position;
								float time_stamp;

								animationbone_line >> position.x >> position.y >> position.z >> time_stamp;
								positions.push_back({ position, time_stamp });
							}

							else if (next_bone_data == "ROTATION")
							{
								glm::quat orientation;
								float time_stamp;

								animationbone_line >> orientation.w >> orientation.x >> orientation.y >> orientation.z >> time_stamp;
								rotations.push_back({ orientation, time_stamp });
							}

							else if (next_bone_data == "SCALE")
							{
								glm::vec3 scale;
								float time_stamp;

								animationbone_line >> scale.x >> scale.y >> scale.z >> time_stamp;
								scales.push_back({ scale, time_stamp });
							}

							else if (next_bone_data == "END")
							{
								float mat_values[16];

								for (int i = 0; i < 16; ++i)
								{
									animationbone_line >> mat_values[i];
								}

								local_transform = { mat_values[0],  mat_values[1],  mat_values[2],  mat_values[3],
													mat_values[4],  mat_values[5],  mat_values[6],  mat_values[7],
													mat_values[8],  mat_values[9],  mat_values[10], mat_values[11],
													mat_values[12], mat_values[13], mat_values[14], mat_values[15] };

								animationbone_line >> name >> id;
							}
						}

						bones.push_back({ positions, rotations, scales, local_transform, name, id });
					}

					else if (next_data == "ROOTNODE")
					{
						glm::mat4 transformation;
						std::string name;

						float mat_values[16];

						for (int i = 0; i < 16; ++i)
						{
							ifs >> mat_values[i];
						}

						transformation = { mat_values[0],  mat_values[1],  mat_values[2],  mat_values[3],
											mat_values[4],  mat_values[5],  mat_values[6],  mat_values[7],
											mat_values[8],  mat_values[9],  mat_values[10], mat_values[11],
											mat_values[12], mat_values[13], mat_values[14], mat_values[15] };

						ifs >> name;

						root_node.transformation = transformation;
						root_node.name = name;
					}

					else if (next_data == "NODEDATA")
					{
						TempNodeData temp;

						ifs >> temp.parent_name;

						float mat_values[16];

						for (int i = 0; i < 16; ++i)
						{
							ifs >> mat_values[i];
						}

						temp.transformation = { mat_values[0],  mat_values[1],  mat_values[2],  mat_values[3],
												mat_values[4],  mat_values[5],  mat_values[6],  mat_values[7],
												mat_values[8],  mat_values[9],  mat_values[10], mat_values[11],
												mat_values[12], mat_values[13], mat_values[14], mat_values[15] };

						ifs >> temp.name;

						node_datas.push_back(temp);
					}

					else if (next_data == "BONEINFO")
					{
						std::string name;
						BoneInfo bone_info;

						ifs >> name >> bone_info.id;
						float mat_values[16];

						for (int i = 0; i < 16; ++i)
						{
							ifs >> mat_values[i];
						}

						bone_info.offset = { mat_values[0],  mat_values[1],  mat_values[2],  mat_values[3],
											 mat_values[4],  mat_values[5],  mat_values[6],  mat_values[7],
											 mat_values[8],  mat_values[9],  mat_values[10], mat_values[11],
											 mat_values[12], mat_values[13], mat_values[14], mat_values[15] };

						bone_info_map[name] = bone_info;
					}

					else if (next_data == "ANIMATIONEND")
					{
						SetUpNodeData(root_node, node_datas);
						model.AddAnimation({ duration, ticks_per_second, bones, root_node, bone_info_map }, anim_name);
						break;
					}
				}
			}

			else if (next == "TYPE")
			{
				int type;
				ifs >> type;
				model.SetPrimitive(type);

				//std::cout << file_path << " has been loaded." << std::endl;
				return model;
			}
		}
	}

	std::cout << "Error in loading " << file_path << std::endl;
	std::exit(EXIT_FAILURE);
}

void NUILoader::SetUpNodeData(NodeData& node, std::vector<TempNodeData>& node_datas)
{
	for (auto& temp_node_data : node_datas)
	{
		if (temp_node_data.parent_name == node.name)
		{
			NodeData child;
			child.name = temp_node_data.name;
			child.transformation = temp_node_data.transformation;

			SetUpNodeData(child, node_datas);
			node.children.push_back({ child });
		}
	}
}

Model::SubMesh NUILoader::CreateSubMesh(std::vector<Model::Vertex> vertices, std::vector<GLushort> indices, std::pair<std::string, TempMaterial> material)
{
	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(Model::Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	// Create a handle for ebo
	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferStorage(ebo, sizeof(GLushort) * indices.size(), reinterpret_cast<GLvoid*>(indices.data()), GL_DYNAMIC_STORAGE_BIT);

	RenderResourceManager::GetInstanced().LoadMaterialNUI(material.first, material.second);

	return std::move(Model::SubMesh{ vertices, indices, vbo, ebo, static_cast<GLuint>(indices.size()), material.first });
}