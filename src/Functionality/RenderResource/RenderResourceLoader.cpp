#include "RenderResourceLoader.h"
#include "Editor/EditorLogger.h"
#include "Editor/EditorLogger_Inline.h"
#include <fstream>

RenderResourceLoader::RenderResourceLoader(): m_Manager{RenderResourceManager::GetInstanced()} {}

void RenderResourceLoader::ReadTextureJson( std::string File, bool Load )
{
	TextureLoad Temp;

	std::ifstream InputFile(File);

	if (InputFile.is_open())
	{
		std::stringstream FileBuffer{};
		FileBuffer << InputFile.rdbuf();
		InputFile.close();

		rapidjson::Document Doc;
		Doc.Parse(FileBuffer.str().c_str());
		const rapidjson::Value& TextureInfo = Doc;

		for (rapidjson::Value::ConstMemberIterator it = TextureInfo.MemberBegin(); it != TextureInfo.MemberEnd(); ++it)
		{
			std::stringstream buffer{};

			Temp.TextureName = it->name.GetString();
			buffer << it->value.GetString();
			buffer >> Temp.TexturePath >> Temp.TextureBool;

			m_Manager.LoadTextures(Temp.TextureName, Temp.TexturePath, Temp.TextureBool);

			if (Load)
				m_LoadedTextures.push_back(Temp);
			else
				m_LevelTextures.push_back(Temp);

			Temp.TextureName.clear();
			Temp.TexturePath.clear();
			Temp.TextureBool = false;
		}
	}
}

void RenderResourceLoader::LoadTextureOnInit()
{
	//goes thru the all folders and loads any not found on the json

	//for editor on load (once) ->this fn is for if user copy files into the texture folder before running the engine

	for (auto& Path : fs::recursive_directory_iterator("../../resources/textures"))
	{
		if (fs::is_regular_file(Path) && Path.path().extension() == ".dds")
		{
			//skip skybox textures & textures generated from loading meshes
			if (Path.path().parent_path() == "../../resources/textures\\Skybox" || Path.path().parent_path() == "../../resources/textures")
				continue;

			CheckAndLoad(Path.path().stem().generic_string().c_str(), Path.path().generic_string().c_str());

		}
	}

	SerializeTextures();
	EDITOR_TRACE_PRINT("All Textures not on the Json has been loaded");

}

void RenderResourceLoader::AddNewTexture()
{
	for (auto& Tex : m_TexturesToLoad)
	{
		CheckAndLoad(Tex.TextureName, Tex.TexturePath);
	}

	SerializeTextures();
}

void RenderResourceLoader::CheckAndLoad( const std::string& Name, const std::string& Path )
{
	if (Name.find("_Mirrored") != std::string::npos)
	{
		std::string Temp = Name.substr(0, Name.find_last_of("_"));

		if (!m_Manager.m_Textures.contains(Temp)) //if the texture isnt found in the texture container
		{
			m_Manager.LoadTextures(Temp, Path, true);
			m_LoadedTextures.push_back({ Temp, Path });

			EDITOR_TRACE_PRINT("Loaded: " + Temp + "Texture");

		}
		else
		{
			if (CheckTexture(Temp, Path)) //if the texture name & path given is the exact same (trying to override file)
			{
				m_Manager.UnloadTextures(Temp);
				RemoveTexture(Temp);
				m_Manager.LoadTextures(Temp, Path, true);
				m_LoadedTextures.push_back({ Temp, Path });

				EDITOR_TRACE_PRINT("Updated: " + Temp + "Texture");
			}
		}

	}
	else
	{
		if (!m_Manager.m_Textures.contains(Name)) //if the texture isnt found in the texture container
		{
			m_Manager.LoadTextures(Name, Path, true);
			m_LoadedTextures.push_back({ Name, Path });

			EDITOR_TRACE_PRINT("Loaded: " + Name + "Texture");

		}
		else 
		{
			if (CheckTexture(Name, Path))
			{
				m_Manager.UnloadTextures(Name);
				RemoveTexture(Name);
				m_Manager.LoadTextures(Name, Path, true);
				m_LoadedTextures.push_back({ Name, Path });

				EDITOR_TRACE_PRINT("Updated: " + Name + "Texture");

			}
		}
	}
}

void RenderResourceLoader::SerializeTextures()
{
	if (m_LoadedTextures.size())
	{
		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> Writer(sb);

		std::ofstream Filestream("../../resources/textureload.texture");

		Writer.StartObject();

		for (auto& Tex : m_LoadedTextures)
		{
			if (Filestream.is_open())
			{
				Writer.Key(Tex.TextureName.c_str());
				Writer.String((Tex.TexturePath + " " + std::to_string(Tex.TextureBool)).c_str());
			}
		}
		Writer.EndObject();
		Filestream << sb.GetString();
	}
}

void RenderResourceLoader::RemoveTexture( std::string TexName )
{
	for (auto it = m_LoadedTextures.begin(); it != m_LoadedTextures.end(); ++it)
	{
		if (it->TextureName == TexName)
		{
			it = m_LoadedTextures.erase(it);
			break;
		}
	}
}

bool RenderResourceLoader::CheckTexture( std::string TexName, std::string Path )
{
	for (auto it = m_LoadedTextures.begin(); it != m_LoadedTextures.end(); ++it)
	{
		if (it->TextureName == TexName && it->TexturePath == Path)
		{
			return true;
		}
	}

	return false;
}


RenderResourceLoader& RenderResourceLoader::GetInstanced()
{
	static RenderResourceLoader rrl;
	return rrl;
}