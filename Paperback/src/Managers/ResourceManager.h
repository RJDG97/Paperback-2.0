#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "PaperbackPCH.h"
#include "Paperback/IManager.h"
#include "Paperback/IComponent.h"

// Redefine value to expand Entity & Archetype Array size
static const Paperback::u32 MAX_ENTITIES = 2000;
static const Paperback::u32 MAX_ARCHETYPES = 30;

namespace Paperback
{
	class PAPERBACK_API ResourceManager
	{
	public:

		friend class Application;

		using ResourceMap = std::unordered_map<std::type_index, Ref<IManager>>;

		ResourceManager();
		~ResourceManager();

		void Init();

		template <typename ManagerType>
		void RegisterManager()
		{
			//PPB_ASSERT_MSG(!(std::is_base_of_v<Manager::IManager, ManagerType>),
			//			 "ResourceManager<ManagerType>::RegisterManager() -> Not a Manager");

			PPB_ASSERT_MSG(managers.find(typeid(ManagerType)) != managers.end(),
						 "ResourceManager<ManagerType>::RegisterManager() -> Manager already registered");

			managers[typeid(ManagerType)] = CreateRef<ManagerType>();
		}

		template <typename ManagerType>
		ManagerType* GetManager()
		{
			PPB_ASSERT_MSG(managers.find(typeid(ManagerType)) == managers.end(),
						 "ResourceManager<ManagerType>::GetManager() -> Manager does not exist");

			auto val = managers.find(typeid(ManagerType));
			return std::reinterpret_pointer_cast<ManagerType>(val->second).get();
		}

	private:

	#pragma warning(disable : 4251)
		ResourceMap managers;
	#pragma warning(default : 4251)
	};
}

#endif