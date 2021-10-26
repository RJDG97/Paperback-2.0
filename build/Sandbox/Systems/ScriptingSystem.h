#pragma once

#include <iostream>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"
#include "Scripts/IScripts.h"

struct scripting_system : paperback::system::instance
{
	// Instance of the system
	Mono* m_pMono = nullptr;

	// Map of entity ID's and Script instances
	std::unordered_map< uint32_t, std::unique_ptr<Script> > scriptlist;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "scripting_system"
	};

	void OnSystemCreated(void) noexcept
	{
		m_pMono = &Mono::GetInstanced();
	}

	void Update(void) noexcept
	{
		tools::query Query;
		Query.m_Must.AddFromComponents<paperback::component::entity, entityscript>();

		// Run each entity with the entity script component
		ForEach(Search(Query), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
			{
				// check for an instance of this entity's script
				auto Found = scriptlist.find(Dynamic_Entity.m_GlobalIndex);

				if (Found == scriptlist.end())
				{	// if no instance create one
					const auto& [Script, Valid] = scriptlist.insert({ Dynamic_Entity.m_GlobalIndex, std::make_unique<Script>() });
					if (Valid)
						Script->second->Init(script.m_ScriptID); // Initialize if successful
					// Run entity's start script
					Script->second->Start();
				}
				else
				{	// run entity's update script
					Found->second->Update(m_Coordinator.DeltaTime());
				}
			});

		// check for updating dll (still needs hot reloading)
		if (PPB.IsKeyPress(GLFW_KEY_C)) {
			// Update Mono DLL
			m_pMono->UpdateDLL();
			// For each entity with a script component, update instance
			for (auto s = scriptlist.begin(); s != scriptlist.end(); s++)
				s->second->ScriptUpdate();
		}
	}

	void OnSystemTerminated(void) noexcept
	{
		m_pMono->ReleaseDomain();
	}
};