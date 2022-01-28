#pragma once

#include "Mono.h"

namespace MONO_MASS
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_mass = m_obj.m_pArchetype->GetComponent<mass>(m_obj.m_PoolDetails);
		return &m_mass;
	}

	MONO_EXPORT float GetMass(void* address)
	{
		if (address)
			return reinterpret_cast<mass*>(address)->m_Mass;

		return {};
	}

	MONO_EXPORT float GetInvMass(void* address)
	{
		if (address)
			return reinterpret_cast<mass*>(address)->m_InvMass;

		return {};
	}

	MONO_EXPORT void SetMass(void* address, float value)
	{
		if (address)
			reinterpret_cast<mass*>(address)->m_Mass = value;
	}

	MONO_EXPORT void SetInvMass(void* address, float value)
	{
		if (address)
			reinterpret_cast<mass*>(address)->m_InvMass = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Mass::getaddress(uint)", &MONO_MASS::GetAddress);
		mono_add_internal_call("CSScript.Mass::getmass(void*)", &MONO_MASS::GetMass);
		mono_add_internal_call("CSScript.Mass::getinvmass(void*)", &MONO_MASS::GetInvMass);
		mono_add_internal_call("CSScript.Mass::setmass(void*,single)", &MONO_MASS::SetMass);
		mono_add_internal_call("CSScript.Mass::setinvmass(void*,single)", &MONO_MASS::SetInvMass);
	}
}