#include "RenderResourceManager.h"

RenderResourceManager::RenderResourceManager()
{
	m_Models["Quad"] = MeshBuilder::Build2DMesh();
	m_Models["Screen"] = MeshBuilder::BuildScreenMesh();
}

RenderResourceManager::~RenderResourceManager()
{
	// Clean up shaders
	for (std::pair<std::string, ShaderProgram> shader : m_Shaders)
		shader.second.DeleteShader();

	m_Shaders.clear();

	// Clean up textures
	UnloadAllTextures();

	// Clean up materials
	UnloadAllMaterials();

	// Clean up models
	UnloadAllMeshes();
}

void RenderResourceManager::LoadShader(const std::string& Name, const std::string& Vert, const std::string& Frag)
{
	if (m_Shaders.find(Name) == m_Shaders.end())
		m_Shaders[Name] = ShaderBuilder::CreateShader(Vert, Frag);
}

void RenderResourceManager::UnloadAllTextures()
{
	for (auto& pair : m_Textures)
		glDeleteTextures(1, &pair.second);

	m_Textures.clear();
}

void RenderResourceManager::UnloadAllMeshes()
{
	for (auto& model : m_Models)
		model.second.RemoveAllSubMesh();

	m_Models.clear();
}

void RenderResourceManager::UnloadAllMaterials()
{
	m_Materials.clear();
}

std::string RenderResourceManager::LoadTextures(const std::string& Texture, const std::string& File)
{
	if (m_Textures.find(Texture) == m_Textures.end())
		m_Textures[Texture] = TextureLoader::LoadTexture(File);

	return Texture;
}

void RenderResourceManager::UnloadTextures(const std::string& Mesh)
{
	glDeleteTextures(1, &m_Textures[Mesh]);
	m_Textures.erase(Mesh);
}

std::string RenderResourceManager::LoadMaterial(const std::string& Material, aiMaterial* AiMat)
{
	if (m_Materials.find(Material) == m_Materials.end())
	{
		std::string file;
		aiString str;
		Model::Material mat;

		AiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		file = "../../resources/textures/" + std::string{ str.C_Str() };
		mat.m_Diffuse = LoadTextures(str.C_Str(), file);

		str.Clear();

		AiMat->GetTexture(aiTextureType_AMBIENT, 0, &str);
		file = "../../resources/textures/" + std::string{ str.C_Str() };
		mat.m_Ambient = LoadTextures(str.C_Str(), file);

		str.Clear();

		AiMat->GetTexture(aiTextureType_SPECULAR, 0, &str);
		file = "../../resources/textures/" + std::string{ str.C_Str() };
		mat.m_Specular = LoadTextures(str.C_Str(), file);

		str.Clear();

		AiMat->GetTexture(aiTextureType_HEIGHT, 0, &str);
		file = "../../resources/textures/" + std::string{ str.C_Str() };
		mat.m_Normal = LoadTextures(str.C_Str(), file);

		m_Materials[Material] = mat;
	}

	return Material;
}

void RenderResourceManager::Load3DMesh(const std::string& Mesh, const std::string& File)
{
	if (m_Models.find(Mesh) == m_Models.end())
		m_Models[Mesh] = MeshBuilder::Build3DMesh(File);
}

void RenderResourceManager::Unload3DMesh(const std::string& Mesh)
{
	if (m_Models.find(Mesh) != m_Models.end())
		m_Models.erase(Mesh);
}

RenderResourceManager& RenderResourceManager::GetInstanced()
{
	static RenderResourceManager rrm;

	return rrm;
}