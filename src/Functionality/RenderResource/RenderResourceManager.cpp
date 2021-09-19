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

void RenderResourceManager::LoadTextures(const std::string& Mesh, const std::string& File)
{
	if (m_Textures.find(Mesh) == m_Textures.end())
		m_Textures[Mesh] = TextureLoader::LoadTexture(File);
}

void RenderResourceManager::UnloadTextures(const std::string& Mesh)
{
	glDeleteTextures(1, &m_Textures[Mesh]);
	m_Textures.erase(Mesh);
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