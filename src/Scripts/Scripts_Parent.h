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

	MONO_EXPORT uint32_t GetChildID(void* address, std::string child_name)
	{
		if (address)
		{
			std::unordered_set<paperback::u32>& ids = reinterpret_cast<parent*>(address)->m_ChildrenGlobalIndexes;

			for (auto id : ids)
			{
				auto m_obj = PPB.GetEntityInfo(id);
				auto& m_name{ m_obj.m_pArchetype->GetComponent<name>(m_obj.m_PoolDetails) };
				
				if (child_name == m_name.m_Value)
					return id;
			}
		}

		return {};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Parent::getaddress(uint)", &MONO_PARENT::GetAddress);
		mono_add_internal_call("CSScript.Parent::getchildid(void*)", &MONO_PARENT::GetChildID);
	}
}