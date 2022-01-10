#pragma once

#include "Mono.h"

namespace MONO_AABB
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_aabb = m_obj.m_pArchetype->GetComponent<aabb>(m_obj.m_PoolDetails);
		return &m_aabb;
	}

	MONO_EXPORT bool GetCollided(void* address)
	{
		return reinterpret_cast<aabb*>(address)->m_Collided;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.AABB::getaddress(uint)", &MONO_AABB::GetAddress);
		mono_add_internal_call("CSScript.AABB::getcollided(void*)", &MONO_AABB::GetCollided);
	}
}