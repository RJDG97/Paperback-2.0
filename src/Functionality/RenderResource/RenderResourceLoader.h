#pragma once
#ifndef RENDER_RESOURCE_LOADER_H
#define RENDER_RESOURCE_LOADER_H

#include <vector>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
#include "rapidjson/document.h"
#include "RenderResourceManager.h"

struct TextureLoad
{
	std::string TextureName{};
	std::string TexturePath{};
	bool TextureBool = false;
	bool Saved = false;
};

struct RenderResourceLoader
{
	RenderResourceLoader();

	public:
	std::vector< TextureLoad > ReadTextureJson( std::string File, bool Load = false );
	
	static RenderResourceLoader& GetInstanced();

	RenderResourceManager& m_Manager;
	std::vector<TextureLoad> m_LevelTextures;
};
#endif