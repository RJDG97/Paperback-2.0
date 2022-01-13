#include "RenderResourceLoader.h"
#include <fstream>

RenderResourceLoader::RenderResourceLoader(): m_Manager{RenderResourceManager::GetInstanced()} {}

void RenderResourceLoader::ReadTextureJson( std::string File, bool Load )
{
	//m_LevelTextures.clear();

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

void RenderResourceLoader::LoadTexture()
{
	//goes thru the all folders and loads any not found on the json

	//for editor on load (once)

	for (auto& Path : fs::recursive_directory_iterator("../../resources/textures"))
	{
		if (fs::is_regular_file(Path) && Path.path().extension() == ".dds")
		{
			if (Path.path().parent_path() == "../../resources/textures\\Skybox" || Path.path().parent_path() == "../../resources/textures")
				continue;

			std::string TexName = Path.path().stem().generic_string().c_str();

			if (TexName.find("_Mirrored") != std::string::npos)
			{
				std::string Temp = TexName.substr(0, TexName.find_last_of("_"));

				if (!m_Manager.m_Textures.contains(Temp)) //if the texture isnt found in the texture container
				{
					m_LoadedTextures.push_back({ Temp, Path.path().generic_string().c_str(), true });
				}
			}
			else
			{
				if (!m_Manager.m_Textures.contains(TexName)) //if the texture isnt found in the texture container
				{
					m_LoadedTextures.push_back({ TexName, Path.path().generic_string().c_str(), true });
				}
			}
		}
	}

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

RenderResourceLoader& RenderResourceLoader::GetInstanced()
{
	static RenderResourceLoader rrl;
	return rrl;
}