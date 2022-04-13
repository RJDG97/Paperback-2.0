/**********************************************************************************
*\file         Prefab.h
*\brief        Prefab.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#ifndef PREFAB_H
#define PREFAB_H

struct prefab
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Prefab"
	};

	void AddPrefabInstance( const paperback::u32 InstanceGID ) noexcept
	{
		m_ReferencePrefabGIDs.emplace( InstanceGID );
	}

	void RemovePrefabInstance( const paperback::u32 InstanceGID ) noexcept
	{
		m_ReferencePrefabGIDs.erase( InstanceGID );
	}

	std::unordered_set<paperback::u32> m_ReferencePrefabGIDs;
};

namespace RR_Prefab
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<prefab>( prefab::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Prefab Instances GID", &prefab::m_ReferencePrefabGIDs);
    }
}

#endif