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

	struct ScriptInfo
	{
		std::string m_ID;
		std::unique_ptr<Script> m_Script;
	};

	std::unordered_multimap<uint32_t, ScriptInfo> scriptlist;
	tools::query m_QueryEntityScripts;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "scripting_system"
	};

	void OnSystemCreated(void) noexcept
	{
		m_pMono = &Mono::GetInstanced();

		m_QueryEntityScripts.m_Must.AddFromComponents<paperback::component::entity, entityscript>();
		m_QueryEntityScripts.m_NoneOf.AddFromComponents<prefab>();
	}

	void Update(void) noexcept
	{
		// Run each entity with the entity script component
		ForEach(Search(m_QueryEntityScripts), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
		{
			// check for an instance of this entity's script
			auto script_component_found = scriptlist.equal_range(Dynamic_Entity.m_GlobalIndex);

			if (script_component_found.first == scriptlist.end()) {

				// if no instance create one
				for (auto script_id : script.m_ScriptID) {

					AddScript(Dynamic_Entity.m_GlobalIndex, script_id);
				}
			}

			else {

				for (auto& script_id : script.m_ScriptID) {

					bool found{};

					for (auto existing_script = script_component_found.first; existing_script != script_component_found.second; ++existing_script) {

						if (existing_script->second.m_ID == script_id)
						{	//current script is not new
							existing_script->second.m_Script->Update(m_Coordinator.DeltaTime());
							found = true;
						}
					}

					//current script is new
					if (!found)
					{
						AddScript(Dynamic_Entity.m_GlobalIndex, script_id);
					}
				}
			}
		});
	}

	void AddScript(uint32_t entity_id, std::string script_id)
	{
		std::unique_ptr<Script> p_script { std::make_unique<Script>() };

		if (p_script)
		{
			p_script->Init(script_id);
			p_script->Start(entity_id);
		}

		scriptlist.emplace(entity_id, ScriptInfo{ script_id, std::move(p_script) });
	}

	void RemoveScript()
	{
		//TODO
	}

	void CompileScripts()
	{
		// Update Mono DLL
		if (m_pMono->UpdateDLL()) {
			// For each entity with a script component, update instance
			for (auto& s : scriptlist) {

				s.second.m_Script->ScriptUpdate();
			}

			DEBUG_PRINT("Compile Success");
		}

		else
			DEBUG_PRINT("Compile Fail");
	}

	void OnSystemTerminated(void) noexcept
	{
		m_pMono->ReleaseDomain();
	}
};