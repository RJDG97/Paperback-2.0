#include "RenderResourceLoader.h"
#include <fstream>

RenderResourceLoader::RenderResourceLoader(): m_Manager{RenderResourceManager::GetInstanced()} {}

std::vector< TextureLoad > RenderResourceLoader::ReadTextureJson( std::string File, bool Load )
{
	//m_LevelTextures.clear();

	TextureLoad Temp;

	std::ifstream InputFile(File);
	std::vector< TextureLoad > TempLoad {};

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
				TempLoad.push_back(Temp);
			else
				m_LevelTextures.push_back(Temp);

			Temp.TextureName.clear();
			Temp.TexturePath.clear();
			Temp.TextureBool = false;
		}
	}

	return TempLoad;
}

RenderResourceLoader& RenderResourceLoader::GetInstanced()
{
	static RenderResourceLoader rrl;
	return rrl;
}