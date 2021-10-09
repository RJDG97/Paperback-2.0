#pragma once

#include <iostream>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"
#include "Scripts/IScripts.h"

struct scripting_system : paperback::system::instance
{
	Mono* m_pMono = nullptr;
	std::unordered_map< std::string, std::unique_ptr<Script> > scriptlist;

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
		Query.m_Must.AddFromComponents<entityscript>();

		ForEach( Search( Query ), [&]( entityscript& script ) noexcept
		{
			auto Found = scriptlist.find( script.m_ScriptID );

			if ( Found == scriptlist.end() )
			{
				const auto& [ Script, Valid ] = scriptlist.insert( { script.m_ScriptID, std::make_unique<Script>( script.m_ScriptID ) } );
				if ( Valid )
					Script->second->Start();
			}
			else
			{
				Found->second->Update();
			}
		});
	}

	void OnSystemTerminated(void) noexcept 
	{
		m_pMono->ReleaseDomain();
	}
};