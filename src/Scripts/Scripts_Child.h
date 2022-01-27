#pragma once

#include "Mono.h"

namespace MONO_CHILD
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_child = m_obj.m_pArchetype->FindComponent<child>(m_obj.m_PoolDetails);
		return m_child;
	}

	MONO_EXPORT int32_t GetParentID(void* address)
	{
		if (address)
			return reinterpret_cast<child*>(address)->m_ParentGlobalIndex;

		return {-1};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Child::getaddress(int)", &MONO_CHILD::GetAddress);
		mono_add_internal_call("CSScript.Child::getparentid(void*)", &MONO_CHILD::GetParentID);
	}
}