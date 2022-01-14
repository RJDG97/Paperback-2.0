#pragma once
#ifndef RENDER_RESOURCE_LOADER_H
#define RENDER_RESOURCE_LOADER_H

#include <vector>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "RenderResourceManager.h"

struct TextureLoad
{
	std::string TextureName{};
	std::string TexturePath{};
	bool TextureBool = true;
	bool Saved = false;
};

struct RenderResourceLoader
{
	RenderResourceLoader();

	public:
	void ReadTextureJson( std::string File, bool Load = false );
	void LoadTextureOnInit();
	void AddNewTexture();
	void CheckAndLoad( const std::string& Name, const std::string& Path );
	void SerializeTextures();
	void RemoveTexture( std::string TexName );

	static RenderResourceLoader& GetInstanced();

	RenderResourceManager& m_Manager;
	std::vector<TextureLoad> m_LevelTextures, m_LoadedTextures, m_TexturesToLoad;
};
#endif