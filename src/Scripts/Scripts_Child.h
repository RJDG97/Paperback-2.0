#pragma once

#include "Mono.h"

namespace MONO_CHILD
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_child = m_obj.m_pArchetype->FindComponent<child>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_child)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Child component." << std::endl;
		}
#endif

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
		mono_add_internal_call("CSScript.Child::getaddress(uint)", &MONO_CHILD::GetAddress);
		mono_add_internal_call("CSScript.Child::getparentid(void*)", &MONO_CHILD::GetParentID);
	}
}