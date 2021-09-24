#pragma once

#include <unordered_map>
#include "glm/inc/glm.hpp"
#include "../Mesh/MeshBuilder.h"
#include "../Shader/ShaderBuilder.h"
#include "../Texture/TextureLoader.h"

class RenderResourceManager
{
public:
	~RenderResourceManager();

	// Helper function to create shaders
	void LoadShader(const std::string& Name, const std::string& Vert, const std::string& Frag);

	// Unloads all textures
	void UnloadAllTextures();
	// Unload all materials
	void UnloadAllMaterials();
	// Unloads all meshes
	void UnloadAllMeshes();

	// Load textures
	std::string LoadTextures(const std::string& Mesh, const std::string& File);
	// Unload textures
	void UnloadTextures(const std::string& Mesh);

	// Load material
	std::string LoadMaterial(const std::string& Material, aiMaterial* AiMat);

	// Load 3D mesh
	void Load3DMesh(const std::string& Mesh, const std::string& File);
	// Unload 3D mesh
	void Unload3DMesh(const std::string& Mesh);

	// Map of 2D textures
	std::unordered_map<std::string, GLuint> m_Textures;
	// Map of Materials
	std::unordered_map<std::string, Model::Material> m_Materials;
	// Mesh of 3D models
	std::unordered_map<std::string, Model> m_Models;
	// Map of shaders
	std::unordered_map<std::string, ShaderProgram> m_Shaders;

	static RenderResourceManager& GetInstanced();
	RenderResourceManager(const RenderResourceManager&) = delete;
	RenderResourceManager& operator=(const RenderResourceManager&) = delete;

private:
	RenderResourceManager();
};