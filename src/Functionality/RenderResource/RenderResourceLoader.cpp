#include "RenderResourceLoader.h"
#include <fstream>

RenderResourceLoader::RenderResourceLoader(): m_Manager{RenderResourceManager::GetInstanced()}
{
	m_LoadedTextures = {};
}

void RenderResourceLoader::ReadAssetJson( const std::string& File, rapidjson::Document& Doc)
{
	std::ifstream InputFile(File);
	std::stringstream Buffer{};
	std::string Input{};

	while (std::getline(InputFile, Input))
	{
		Buffer << Input << "\n";
	}
	InputFile.close();

	Doc.Parse(Buffer.str().c_str());
}

void RenderResourceLoader::ReadTextureJson( std::string File )
{
	TextureLoad Temp;
	rapidjson::Document Doc;

	ReadAssetJson(File, Doc);
	const rapidjson::Value& TextureInfo = Doc;
	//add assert

	for (rapidjson::Value::ConstMemberIterator it = TextureInfo.MemberBegin(); it != TextureInfo.MemberEnd(); ++it)
	{
		std::stringstream buffer{};

		Temp.TextureName = it->name.GetString();
		buffer << it->value.GetString();
		buffer >> Temp.TexturePath >> Temp.TextureBool;

		m_Manager.LoadTextures(Temp.TextureName, Temp.TexturePath, Temp.TextureBool);
		m_LoadedTextures.push_back(Temp);

		Temp.TextureName.clear(); 
		Temp.TexturePath.clear(); 
		Temp.TextureBool = false;
	}
}

RenderResourceLoader& RenderResourceLoader::GetInstanced()
{
	static RenderResourceLoader rrl;
	return rrl;
}