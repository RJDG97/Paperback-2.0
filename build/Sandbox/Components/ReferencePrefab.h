#pragma once
#ifndef REFERENCE_PREFAB_H
#define REFERENCE_PREFAB_H

#include "paperback_bitset.h"

struct reference_prefab
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "ReferencePrefab"
	};

	void AddModifiedComponentGuid( const paperback::u64 ComponentGuid ) noexcept
	{
		m_ModifiedComponents.emplace( ComponentGuid );
	}

	void RemoveModifiedComponentGuid( const paperback::u64 ComponentGuid ) noexcept
	{
		m_ModifiedComponents.erase( ComponentGuid );
	}

	// Tells the system which components should be saved and
	// which components should use default values from the prefab
	std::unordered_set<paperback::u64>	   m_ModifiedComponents;	// For Tracking Non-Default Component Values - Stores m_Guid.m_Value
	paperback::u32                         m_PrefabGID;			    // For Resetting Values to "Default" & Obtaining reference to Prefab's EntityInfo
};

namespace RR_ReferencePrefab
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<reference_prefab>( reference_prefab::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Modified Components", &reference_prefab::m_ModifiedComponents )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Prefab GID", &reference_prefab::m_PrefabGID )( rttr::policy::prop::as_reference_wrapper );
    }
}

#endif