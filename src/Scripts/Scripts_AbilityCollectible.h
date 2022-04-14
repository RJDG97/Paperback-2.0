/**********************************************************************************
*\file         Scripts_AbilityCollectible.h
*\brief        Scripts_AbilityCollectible.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

	MONO_EXPORT bool GetGrowStatus(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<ability_collectible>(m_obj.m_PoolDetails)->m_Grow;

		return false;
	}

	MONO_EXPORT bool GetShrinkStatus(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<ability_collectible>(m_obj.m_PoolDetails)->m_Shrink;

		return false;
	}

	MONO_EXPORT bool GetFreezeStatus(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<ability_collectible>(m_obj.m_PoolDetails)->m_Freeze;

		return false;
	}

	MONO_EXPORT void SetGrowStatus(uint32_t ID, bool status )
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<ability_collectible>(m_obj.m_PoolDetails)->m_Grow = status;
	}

	MONO_EXPORT void SetShrinkStatus(uint32_t ID, bool status )
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<ability_collectible>(m_obj.m_PoolDetails)->m_Shrink = status;
	}

	MONO_EXPORT void SetFreezeStatus(uint32_t ID, bool status )
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<ability_collectible>(m_obj.m_PoolDetails)->m_Freeze = status;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.AbilityCollectible::getaddress(uint)",       &MONO_ABILITYCOLLECTIBLE::GetAddress);
		mono_add_internal_call("CSScript.AbilityCollectible::GetGrowStatus(uint)",   &MONO_ABILITYCOLLECTIBLE::GetGrowStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::GetShrinkStatus(uint)", &MONO_ABILITYCOLLECTIBLE::GetShrinkStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::GetFreezeStatus(uint)", &MONO_ABILITYCOLLECTIBLE::GetFreezeStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::SetGrowStatus(uint,bool)", &MONO_ABILITYCOLLECTIBLE::SetGrowStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::SetShrinkStatus(uint,bool)", &MONO_ABILITYCOLLECTIBLE::SetShrinkStatus);
		mono_add_internal_call("CSScript.AbilityCollectible::SetFreezeStatus(uint,bool)", &MONO_ABILITYCOLLECTIBLE::SetFreezeStatus);
	}
}