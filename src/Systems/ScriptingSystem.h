#pragma once

#include <iostream>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"
#include "Scripts/IScripts.h"

struct scripting_system : paperback::system::instance
{
	Mono* m_pMono = nullptr;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "scripting_system"
	};

	void OnSystemCreated(void) noexcept
	{
		// Set up Mono
		m_pMono = &Mono::GetInstanced();	
	}

	//void operator()(transform& Transform, paperback::component::entity& Entity) noexcept
	//{
	//	static std::unordered_map<std::string, Script*> scriptlist;

	//	// Check collisions
	//	tools::query Query;
	//	Query.m_Must.AddFromComponents<transform, entityscript>();

	//	ForEach(Search(Query), [&](entityscript& script) noexcept
	//	{
	//		std::unordered_map<std::string, Script*>::const_iterator found = scriptlist.find(script.m_ScriptID);
	//		if (found == scriptlist.end()) {
	//			scriptlist.insert({ script.m_ScriptID, new Script(script.m_ScriptID) });
	//		}
	//		else {
	//			found->second->Start();
	//		}
	//	});
	//}

	void Update(void) noexcept 
	{
		m_pMono->RunImportFn(m_pMono->m_pMainObj, m_pMono->m_pMainFn);

		//static std::unordered_map<std::string, Script*> scriptlist;

		//tools::query Query;
		//Query.m_Must.AddFromComponents<entityscript>();

		//for (auto& Archetype : Search(Query)) {
		//	std::cout << "Entity Count: " << Archetype->GetEntityCount() << std::endl;
		//	std::cout << "Component Pool: " << Archetype->GetComponentPools()[0].GetCurrentEntityCount() << std::endl;
		//	
		//}

		//ForEach(Search(Query), [&](paperback::component::entity& Dynamic_Entity, entityscript& script) noexcept
		//{
		//	ERROR_LOG("Checking");
		//	std::unordered_map<std::string, Script*>::const_iterator found = scriptlist.find(script.m_ScriptID);
		//	if (found == scriptlist.end()) {
		//		scriptlist.insert({ script.m_ScriptID, new Script(script.m_ScriptID) });
		//	}
		//	else {
		//		found->second->Start();
		//	}
		//});
	}

	void OnSystemTerminated(void) noexcept 
	{
		delete m_pMono;
	}
};