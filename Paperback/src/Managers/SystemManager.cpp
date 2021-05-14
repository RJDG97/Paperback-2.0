#include "PaperbackPCH.h"
#include "SystemManager.h"

namespace Paperback
{
	SystemManager::SystemManager()
	{

	}

	SystemManager::~SystemManager()
	{

	}

	void SystemManager::Init()
	{
		for (auto& [system_name, system] : systems)
		{
			system->Init();
		}
	}

	void SystemManager::EntityDeleted(EntityID id)
	{
		for (auto& [system_name, system] : systems)
		{
			system->RemoveEntity(id);
		}
	}

	void SystemManager::EntitySignatureUpdated(EntityID id, Signature sig)
	{
		for (auto& [system_name, system] : systems)
		{
			auto sys_signature = system_signatures[system_name];

			// Entity Signature matches System's Signature
			if ((sig & sys_signature) == sys_signature)
			{
				system->RegisterEntity(id);
			}
			// No match
			else
			{
				system->RemoveEntity(id);
			}
		}
	}
}