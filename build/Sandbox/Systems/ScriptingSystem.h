/**********************************************************************************
*\file         ScriptingSystem.h
*\brief        ScriptingSystem.h
*
*\author       Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <iostream>
#include <map>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"
#include "Scripts/IScripts.h"

struct scripting_system : paperback::system::pausable_instance
{
	// Instance of the system
	Mono* m_pMono = nullptr;

	/*struct ScriptsInfo
	{
		std::vector<std::pair<std::string, std::unique_ptr<Script>>> m_Info;
	};*/

	//std::map<uint32_t, ScriptsInfo> scriptlist;
	std::map<uint32_t, std::unique_ptr<Script>> scriptlist;
	tools::query m_QueryEntityScripts;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "scripting_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		m_pMono = &Mono::GetInstanced();

		m_QueryEntityScripts.m_Must.AddFromComponents<paperback::component::entity, entityscript>();
		m_QueryEntityScripts.m_NoneOf.AddFromComponents<prefab>();
	} 

	PPB_INLINE
	void OnStateChange( void ) noexcept
	{
		scriptlist.clear();
	}

	PPB_INLINE
	void OnStateLoad( void ) noexcept
	{
		// Run each entity with the entity script component
		ForEach( Search( m_QueryEntityScripts ), [&]( paperback::component::entity& Dynamic_Entity, entityscript& script ) noexcept
		{
			// check for an instance of this entity's script
			auto entry_found = scriptlist.find( Dynamic_Entity.m_GlobalIndex );

			if ( entry_found == scriptlist.end() )
			{
				AddScript( Dynamic_Entity.m_GlobalIndex, script.m_ScriptID );
			}
		});
	}


	PPB_FORCEINLINE
	void PreUpdate(void) noexcept
	{
		m_Coordinator.SetTotalProcesses(0);
		m_Coordinator.SetProcessesCompleted(0);

		// Run each entity with the entity script component
		ForEach(Search(m_QueryEntityScripts), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
		{
			// check for an instance of this entity's script
			auto entry_found = scriptlist.find(Dynamic_Entity.m_GlobalIndex);

			if (entry_found != scriptlist.end()) {

			//	AddScript(Dynamic_Entity.m_GlobalIndex, script.m_ScriptID);
			//}

			//else {

				//for (auto& to_update : entry_found->second.m_Info)
				////{
				//	m_Coordinator.Increment_TotalProcesses();
					entry_found->second->PreUpdate(m_Coordinator.DeltaTime());/*
				}*/
			}
		});/*

		while (!m_Coordinator.CompareProcesses())
		{
		}*/
	}

	PPB_FORCEINLINE
	void Update(void) noexcept
	{
	}

	PPB_FORCEINLINE
	void PostUpdate(void) noexcept
	{
		m_Coordinator.SetTotalProcesses(0);
		m_Coordinator.SetProcessesCompleted(0);

		// Run each entity with the entity script component
		ForEach(Search(m_QueryEntityScripts), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
		{
			// check for an instance of this entity's script
			auto entry_found = scriptlist.find(Dynamic_Entity.m_GlobalIndex);

			if (entry_found != scriptlist.end()) {/*

				AddScript(Dynamic_Entity.m_GlobalIndex, script.m_ScriptID);
			}

			else {*/

					//m_Coordinator.Increment_TotalProcesses();
					entry_found->second->Update(m_Coordinator.DeltaTime());
			}
		});/*

		while (!m_Coordinator.CompareProcesses())
		{
		}*/
	}

	void AddScript(uint32_t entity_id, std::vector<entityscript::ScriptID> script_ids)
	{
		if (script_ids.size() == 1)
		{
			std::unique_ptr<Script> p_script{ std::make_unique<Script>() };

			if (p_script)
			{
				p_script->Init(script_ids[0].m_ID);
				p_script->Start(entity_id);

				scriptlist.insert({ entity_id, std::move(p_script) });
			}
		}
	}

	void CompileScripts()
	{
		// Update Mono DLL
		if (m_pMono->UpdateDLL()) {
			// For each entity with a script component, update instance
			for (auto& s : scriptlist) {
				s.second->ScriptUpdate();
			}

			DEBUG_PRINT("Compile Success");
		}

		else
			DEBUG_PRINT("Compile Fail");
	}

	void CallReset()
	{
		ForEach(Search(m_QueryEntityScripts), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
		{
			// check for an instance of this entity's script
			auto entry_found = scriptlist.find(Dynamic_Entity.m_GlobalIndex);

			if (entry_found == scriptlist.end()) {

				AddScript(Dynamic_Entity.m_GlobalIndex, script.m_ScriptID);
			}

			else {

				entry_found->second->Reset();
			}
		});
	}

	void OnSystemTerminated(void) noexcept
	{
		m_pMono->ReleaseDomain();
	}
};