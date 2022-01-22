#pragma once

#include <iostream>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"
#include "Scripts/IScripts.h"

struct scripting_system : paperback::system::pausable_instance
{
	// Instance of the system
	Mono* m_pMono = nullptr;

	struct ScriptsInfo
	{
		std::vector<std::pair<std::string, std::unique_ptr<Script>>> m_Info;
	};

	std::map<uint32_t, ScriptsInfo> scriptlist;
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

	void OnPause(const bool&) noexcept
	{
		scriptlist.clear();

		// Run each entity with the entity script component
		ForEach(Search(m_QueryEntityScripts), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
		{
			// check for an instance of this entity's script
			auto entry_found = scriptlist.find(Dynamic_Entity.m_GlobalIndex);

			if (entry_found == scriptlist.end()) {

				AddScript(Dynamic_Entity.m_GlobalIndex, script.m_ScriptID);
			}
		});
	}

	void Update(void) noexcept
	{
		std::vector<uint32_t> updated_script_entries;

		// Run each entity with the entity script component
		ForEach(Search(m_QueryEntityScripts), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
		{
			// check for an instance of this entity's script
			auto entry_found = scriptlist.find(Dynamic_Entity.m_GlobalIndex);

			if (entry_found == scriptlist.end()) {

				AddScript(Dynamic_Entity.m_GlobalIndex, script.m_ScriptID);
			}

			else {

				for (auto& to_update : entry_found->second.m_Info)
				{
					to_update.second->Update(m_Coordinator.DeltaTime());
				}
			}

			updated_script_entries.push_back(Dynamic_Entity.m_GlobalIndex);
		});

		//destroy all scripts and remove all entries that are no longer updated
		std::map<uint32_t, ScriptsInfo>::iterator to_remove{ scriptlist.end() };

		for (auto entry = scriptlist.begin() ; entry != scriptlist.end() ; ++entry)
		{
			if (to_remove != scriptlist.end())
			{
				scriptlist.erase(to_remove);
			}

			if (std::find(updated_script_entries.begin(), updated_script_entries.end(), entry->first) == updated_script_entries.end())
			{
				for (auto& to_destroy : entry->second.m_Info)
				{
					to_destroy.second->Destroy();
				}

				to_remove = entry;
			}
		}
	}

	void AddScript(uint32_t entity_id, std::vector<std::string> script_ids)
	{
		ScriptsInfo new_info{};

		for (auto& script_id : script_ids)
		{
			std::unique_ptr<Script> p_script{ std::make_unique<Script>() };

			if (p_script)
			{
				p_script->Init(script_id);
				p_script->Start(entity_id);
				new_info.m_Info.push_back({ script_id, std::move(p_script) });
			}
		}

		scriptlist.insert({ entity_id, std::move(new_info) });
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
				for (auto& i : s.second.m_Info)
					i.second->ScriptUpdate();
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