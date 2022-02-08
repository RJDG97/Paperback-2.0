#pragma once

#include "Mono.h"

namespace MONO_AABB
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_aabb = m_obj.m_pArchetype->FindComponent<aabb>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_aabb)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no AABB component." << std::endl;
		}
#endif

		return m_aabb;
	}

	MONO_EXPORT bool GetCollided(void* address)
	{
		if (address)
			return reinterpret_cast<aabb*>(address)->m_Collided;

		return {};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.AABB::getaddress(uint)", &MONO_AABB::GetAddress);
		mono_add_internal_call("CSScript.AABB::getcollided(void*)", &MONO_AABB::GetCollided);
	}
}