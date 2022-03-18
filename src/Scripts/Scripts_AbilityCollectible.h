#pragma once

#include "Mono.h"

namespace MONO_ABILITYCOLLECTIBLE
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_abilitycollectible = m_obj.m_pArchetype->FindComponent<ability_collectible>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_abilitycollectible)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no AbilityCollectible component." << std::endl;
		}
#endif

		return m_abilitycollectible;
	}

	MONO_EXPORT bool GetGrowStatus( void* address )
	{
		if (address)
			return reinterpret_cast<ability_collectible*>(address)->m_Grow;

		return false;
	}

	MONO_EXPORT bool GetShrinkStatus( void* address )
	{
		if (address)
			return reinterpret_cast<ability_collectible*>(address)->m_Shrink;

		return false;
	}

	MONO_EXPORT bool GetFreezeStatus( void* address )
	{
		if (address)
			return reinterpret_cast<ability_collectible*>(address)->m_Freeze;

		return false;
	}

	MONO_EXPORT void SetGrowStatus( void* address, bool status )
	{
		if (address)
			reinterpret_cast<ability_collectible*>(address)->m_Grow = status;
	}

	MONO_EXPORT void SetShrinkStatus( void* address, bool status )
	{
		if (address)
			reinterpret_cast<ability_collectible*>(address)->m_Shrink = status;
	}

	MONO_EXPORT void SetFreezeStatus( void* address, bool status )
	{
		if (address)
			reinterpret_cast<ability_collectible*>(address)->m_Freeze = status;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.AbilityCollectible::getaddress(uint)",       &MONO_ABILITYCOLLECTIBLE::GetAddress);
		mono_add_internal_call("CSScript.AbilityCollectible::GetGrowStatus(void*)",   &MONO_ABILITYCOLLECTIBLE::GetGrowStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::GetShrinkStatus(void*)", &MONO_ABILITYCOLLECTIBLE::GetShrinkStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::GetFreezeStatus(void*)", &MONO_ABILITYCOLLECTIBLE::GetFreezeStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::SetGrowStatus(void*,bool)", &MONO_ABILITYCOLLECTIBLE::SetGrowStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::SetShrinkStatus(void*,bool)", &MONO_ABILITYCOLLECTIBLE::SetShrinkStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::SetFreezeStatus(void*,bool)", &MONO_ABILITYCOLLECTIBLE::SetFreezeStatus);
	}
}