/**********************************************************************************
*\file         RenderResourceManager.h
*\brief        RenderResourceManager.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include <unordered_map>
#include "glm/inc/glm.hpp"
#include "../Mesh/MeshBuilder.h"
#include "../Shader/ShaderBuilder.h"
#include "../Texture/TextureLoader.h"
#include "../Font/Font.h"
#include "../NUI/NUILoader.h"

class RenderResourceManager
{
public:
	~RenderResourceManager();

	// Helper function to create shaders
	void LoadShader(const std::string& Name, const std::string& Vert, const std::string& Frag);

	// Unloads all fonts
	void UnloadAllFonts();
	// Unloads all textures
	void UnloadAllTextures();
	// Unload all materials
	void UnloadAllMaterials();
	// Unloads all meshes
	void UnloadAllMeshes();

	void Load3DMeshNUI(const std::string& Mesh, const std::string& File);
	void LoadMaterialNUI(const std::string& material_name, NUILoader::TempMaterial& material_data);

	// Load fonts
	void LoadFonts(const std::string& Name, const std::string& File);

	// Load textures
	std::string LoadTextures(const std::string& Texture, const std::string& File, const bool& GammaCorrect);
	// Unload textures
	void UnloadTextures(const std::string& Mesh);

	void LoadSkyboxTexture(const std::vector<std::string>& Textures);

	// Load material
	std::string LoadMaterial(const std::string& Material, aiMaterial* AiMat);

	// Load 3D mesh
	void Load3DMesh(const std::string& Mesh, const std::string& File);
	// Unload 3D mesh
	void Unload3DMesh(const std::string& Mesh);

	// Map of fonts
	std::unordered_map<std::string, Font> m_Fonts;
	// Map of 2D textures
	std::unordered_map<std::string, GLuint> m_Textures;
	// Map of Materials
	std::unordered_map<std::string, Model::Material> m_Materials;
	// Mesh of 3D models
	std::unordered_map<std::string, Model> m_Models;
	// Map of shaders
	std::unordered_map<std::string, ShaderProgram> m_Shaders;

	GLuint m_Skybox;
	GLuint m_SkyboxVBO;

	static RenderResourceManager& GetInstanced();
	RenderResourceManager(const RenderResourceManager&) = delete;
	RenderResourceManager& operator=(const RenderResourceManager&) = delete;

private:
	RenderResourceManager();
};