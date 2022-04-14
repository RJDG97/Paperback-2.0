/**********************************************************************************
*\file         Scripts_Freezable.h
*\brief        Scripts_Freezable.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_FREEZABLE
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_freezable = m_obj.m_pArchetype->FindComponent<freezable>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_freezable)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Freezable component." << std::endl;
		}
#endif

		return m_freezable;
	}

	MONO_EXPORT bool GetFrozen(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<freezable>(m_obj.m_PoolDetails)->m_Frozen;

		return {};
	}

	MONO_EXPORT void SetFrozen(uint32_t ID, bool value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<freezable>(m_obj.m_PoolDetails)->m_Frozen = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Freezable::getaddress(uint)", &MONO_FREEZABLE::GetAddress);
		mono_add_internal_call("CSScript.Freezable::getfrozen(uint)", &MONO_FREEZABLE::GetFrozen);
		mono_add_internal_call("CSScript.Freezable::setfrozen(uint,bool)", &MONO_FREEZABLE::SetFrozen);
	}
}