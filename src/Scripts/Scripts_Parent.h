/**********************************************************************************
*\file         Scripts_Parent.h
*\brief        Scripts_Parent.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_PARENT
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_parent = m_obj.m_pArchetype->FindComponent<parent>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_parent)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Parent component." << std::endl;
		}
#endif

		return m_parent;
	}

	MONO_EXPORT int32_t GetChildIDOfName(uint32_t ID, MonoString* child_name)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			std::unordered_set<paperback::u32>& ids = m_obj.m_pArchetype->FindComponent<parent>(m_obj.m_PoolDetails)->m_ChildrenGlobalIndexes;

			for (auto id : ids)
			{
				auto m_obj = PPB.GetEntityInfo(id);
				auto& m_name{ m_obj.m_pArchetype->GetComponent<name>(m_obj.m_PoolDetails) };
				
				if (mono_string_to_utf8(child_name) == m_name.m_Value)
					return id;
			}
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Parent::getaddress(uint)", &MONO_PARENT::GetAddress);
		mono_add_internal_call("CSScript.Parent::getchildidofname(uint,string)", &MONO_PARENT::GetChildIDOfName);
	}
}