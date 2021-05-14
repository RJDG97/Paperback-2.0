#pragma once
#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "PaperbackPCH.h"
#include "Paperback/ISystem.h"

namespace Paperback
{
	class PAPERBACK_API SystemManager
	{
	public:

		friend class Application;

		using SystemMap = std::vector<std::pair<std::string, Ref<ISystem>>>;
		using SystemSignatures = std::unordered_map<std::string, Signature>;

		SystemManager();
		~SystemManager();

		void Init();
		void EntityDeleted(EntityID id);
		void EntitySignatureUpdated(EntityID id, Signature sig);

		template <typename SystemType>
		void RegisterSystem()
		{
			auto system_name = typeid(SystemType).name();

			//PPB_ASSERT_MSG(!(std::is_base_of_v<System::ISystem, SystemType>),
			//			 "SystemManager<SystemType>::RegisterSystem() -> Not a System");

			for (auto& [id, system] : systems)
			{
				PPB_ASSERT_MSG(id == system_name,
							 "SystemManager<SystemType>::RegisterSystem() -> System already registered");
			}

			systems.push_back({ system_name, CreateRef<SystemType>() });
		}

		template <typename SystemType>
		typename SystemType* GetSystem()
		{
			auto system_name = typeid(SystemType).name();

			//PPB_ASSERT_MSG(!(std::is_base_of_v<System::ISystem, SystemType>),
			//			 "SystemManager<SystemType>::GetSystem() -> Not a System");

			for (auto& [id, system] : systems)
			{
				if (id == system_name)
					return std::reinterpret_pointer_cast<SystemType>(system).get();
			}

			PPB_ASSERT_MSG(true, "SystemManager<SystemType>::GetSystem() -> System does not exist");
		}

		template <typename SystemType>
		void SetSignature(Signature signature)
		{
			auto system_name = typeid(SystemType).name();

			PPB_ASSERT_MSG(system_signatures.find(system_name) != system_signatures.end(),
						 "SystemManager<SystemType>::SetSignature() -> System signature already initialized");

			for (auto& [id, system_signature] : system_signatures)
			{
				if (id == system_name)
					return;
			}

			system_signatures.insert({ system_name, signature });
		}

	private:

	#pragma warning(disable : 4251)
		SystemMap systems;
		SystemSignatures system_signatures;
	#pragma warning(default : 4251)
	};
}

#endif