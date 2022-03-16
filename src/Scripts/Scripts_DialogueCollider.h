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

	MONO_EXPORT MonoString* GetDialogueName(void* address)
	{
		if (address)
			return mono_string_new(mono_domain_get(), reinterpret_cast<dialogue_collider*>(address)->m_DialogueName.c_str());

		return {};
	}

	MONO_EXPORT void SetDialogueName(void* address, MonoString* value)
	{
		if (address)
			reinterpret_cast<dialogue_collider*>(address)->m_DialogueName = mono_string_to_utf8(value);
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.DialogueCollider::getaddress(uint)", &MONO_DIALOGUECOLLIDER::GetAddress);
		mono_add_internal_call("CSScript.DialogueCollider::getdialoguename(void*)", &MONO_DIALOGUECOLLIDER::GetDialogueName);
		mono_add_internal_call("CSScript.DialogueCollider::setdialoguename(void*,string)", &MONO_DIALOGUECOLLIDER::SetDialogueName);
	}
}