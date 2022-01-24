#pragma once

#include "Mono.h"

namespace MONO_CHILD
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_child = m_obj.m_pArchetype->GetComponent<child>(m_obj.m_PoolDetails);
		return &m_child;
	}

	MONO_EXPORT uint32_t GetParentID(void* address)
	{
		if (address)
			return reinterpret_cast<child*>(address)->m_ParentGlobalIndex;

		return {};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Child::getaddress(uint)", &MONO_CHILD::GetAddress);
		mono_add_internal_call("CSScript.Child::getparentid(void*)", &MONO_CHILD::GetParentID);
	}
}