#include "PaperbackPCH.h"
#include "ResourceManager.h"

namespace Paperback
{
	ResourceManager::ResourceManager()
	{
		
	}

	ResourceManager::~ResourceManager()
	{
		
	}

	void ResourceManager::Init()
	{
		for (auto& [manager_name, manager] : managers)
		{
			manager->Init();
		}
	}
}