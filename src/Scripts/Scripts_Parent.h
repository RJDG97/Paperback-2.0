#pragma once

#include "Mono.h"

namespace MONO_PARENT
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_parent = m_obj.m_pArchetype->GetComponent<parent>(m_obj.m_PoolDetails);
		return &m_parent;
	}

	MONO_EXPORT uint32_t GetChildID(void* address)
	{
		if (address)
			return reinterpret_cast<parent*>(address)->m_ChildGlobalIndex;

		return {};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Parent::getaddress(uint)", &MONO_PARENT::GetAddress);
		mono_add_internal_call("CSScript.Parent::getchildid(void*)", &MONO_PARENT::GetChildID);
	}
}