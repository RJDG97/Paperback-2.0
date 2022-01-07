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
};

struct RenderResourceLoader
{
	RenderResourceLoader();

	public:
	void ReadAssetJson( const std::string& File, rapidjson::Document& Doc );
	void ReadTextureJson( std::string File );
	
	static RenderResourceLoader& GetInstanced();

	//probs store the path in a container over here
	std::vector< TextureLoad > m_LoadedTextures;
	RenderResourceManager& m_Manager;
};
#endif