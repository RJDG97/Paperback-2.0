/**********************************************************************************
*\file         Scripts_DialogueCollider.h
*\brief        Scripts_DialogueCollider.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_DIALOGUECOLLIDER
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_dialoguecollider = m_obj.m_pArchetype->FindComponent<dialogue_collider>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_dialoguecollider)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Dialogue Collider component." << std::endl;
		}
#endif

		return m_dialoguecollider;
	}

	MONO_EXPORT MonoString* GetDialogueName(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return mono_string_new(mono_domain_get(), m_obj.m_pArchetype->FindComponent<dialogue_collider>(m_obj.m_PoolDetails)->m_DialogueName.c_str());

		return {};
	}

	MONO_EXPORT void SetDialogueName(uint32_t ID, MonoString* value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<dialogue_collider>(m_obj.m_PoolDetails)->m_DialogueName = mono_string_to_utf8(value);
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.DialogueCollider::getaddress(uint)", &MONO_DIALOGUECOLLIDER::GetAddress);
		mono_add_internal_call("CSScript.DialogueCollider::getdialoguename(uint)", &MONO_DIALOGUECOLLIDER::GetDialogueName);
		mono_add_internal_call("CSScript.DialogueCollider::setdialoguename(uint,string)", &MONO_DIALOGUECOLLIDER::SetDialogueName);
	}
}