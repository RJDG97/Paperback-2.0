#pragma once

#include <iostream>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"
#include "Scripts/IScripts.h"

struct scripting_system : paperback::system::instance
{
	Mono* m_pMono = nullptr;

	std::unordered_map<std::string, Script*> scriptlist;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "scripting_system"
	};

	void OnSystemCreated(void) noexcept
	{
		// Set up Mono
		m_pMono = &Mono::GetInstanced();
	}

	void Update(void) noexcept 
	{
		tools::query Query;
		Query.m_Must.AddFromComponents<entityscript>();

		ForEach(Search(Query), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
		{
			std::unordered_map<std::string, Script*>::const_iterator found = scriptlist.find(script.m_ScriptID);
			if (found == scriptlist.end()) {
				scriptlist.insert({ script.m_ScriptID, new Script(script.m_ScriptID) });
				found = scriptlist.find(script.m_ScriptID);
				found->second->Start();
			}
			else {
				found->second->Update();
			}
		});
	}

	void OnSystemTerminated(void) noexcept 
	{
		for (auto temp : scriptlist)
			delete temp.second;

		m_pMono->ReleaseDomain();
	}
};