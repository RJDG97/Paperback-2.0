/**********************************************************************************
*\file         Scripts_Mass.h
*\brief        Scripts_Mass.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_MASS
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_mass = m_obj.m_pArchetype->FindComponent<mass>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_mass)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Mass component." << std::endl;
		}
#endif

		return m_mass;
	}

	MONO_EXPORT float GetMass(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<mass>(m_obj.m_PoolDetails)->m_Mass;

		return {};
	}

	MONO_EXPORT float GetInvMass(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<mass>(m_obj.m_PoolDetails)->m_InvMass;

		return {};
	}

	MONO_EXPORT void SetMass(uint32_t ID, float value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<mass>(m_obj.m_PoolDetails)->m_Mass = value;
	}

	MONO_EXPORT void SetInvMass(uint32_t ID, float value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<mass>(m_obj.m_PoolDetails)->m_InvMass = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Mass::getaddress(uint)", &MONO_MASS::GetAddress);
		mono_add_internal_call("CSScript.Mass::getmass(uint)", &MONO_MASS::GetMass);
		mono_add_internal_call("CSScript.Mass::getinvmass(uint)", &MONO_MASS::GetInvMass);
		mono_add_internal_call("CSScript.Mass::setmass(uint,single)", &MONO_MASS::SetMass);
		mono_add_internal_call("CSScript.Mass::setinvmass(uint,single)", &MONO_MASS::SetInvMass);
	}
}