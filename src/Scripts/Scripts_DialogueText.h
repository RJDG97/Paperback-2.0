#pragma once

#include "Mono.h"

namespace MONO_DIALOGUETEXT
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_dialoguetext = m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_dialoguetext)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Dialogue Text component." << std::endl;
		}
#endif

		return m_dialoguetext;
	}

	MONO_EXPORT MonoString* GetDialogueName(void* address)
	{
		if (address)
			return mono_string_new(mono_domain_get(), reinterpret_cast<dialogue_text*>(address)->m_DialogueName.c_str());

		return {};
	}

	MONO_EXPORT void SetDialogueName(void* address, MonoString* value)
	{
		if (address)
			reinterpret_cast<dialogue_text*>(address)->m_DialogueName = mono_string_to_utf8(value);
	}

	MONO_EXPORT float GetElapsedTime(void* address)
	{
		if (address)
			return reinterpret_cast<dialogue_text*>(address)->m_ElapsedTime;

		return {};
	}

	MONO_EXPORT void SetElapsedTime(void* address, float value)
	{
		if (address)
			reinterpret_cast<dialogue_text*>(address)->m_ElapsedTime = value;
	}

	MONO_EXPORT uint32_t GetState(void* address)
	{
		if (address)
			return reinterpret_cast<dialogue_text*>(address)->m_State;

		return {};
	}

	MONO_EXPORT void SetState(void* address, uint32_t value)
	{
		if (address)
			reinterpret_cast<dialogue_text*>(address)->m_State = value;
	}

	MONO_EXPORT paperback::Vector3f GetInitialScale(void* address)
	{
		if (address)
			return reinterpret_cast<dialogue_text*>(address)->m_InitialScale;

		return {};
	}

	MONO_EXPORT void SetInitialScale(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<dialogue_text*>(address)->m_InitialScale = { x, y, z };
	}

	MONO_EXPORT uint32_t GetIndex(void* address)
	{
		if (address)
			return reinterpret_cast<dialogue_text*>(address)->m_CurrentIndex;

		return {};
	}

	MONO_EXPORT void SetIndex(void* address, uint32_t value)
	{
		if (address)
			reinterpret_cast<dialogue_text*>(address)->m_CurrentIndex = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.DialogueText::getaddress(uint)", &MONO_DIALOGUETEXT::GetAddress);
		mono_add_internal_call("CSScript.DialogueText::getdialoguename(void*)", &MONO_DIALOGUETEXT::GetDialogueName);
		mono_add_internal_call("CSScript.DialogueText::setdialoguename(void*,string)", &MONO_DIALOGUETEXT::SetDialogueName);
		mono_add_internal_call("CSScript.DialogueText::getelapsedtime(void*)", &MONO_DIALOGUETEXT::GetElapsedTime);
		mono_add_internal_call("CSScript.DialogueText::setelapsedtime(void*,single)", &MONO_DIALOGUETEXT::SetElapsedTime);
		mono_add_internal_call("CSScript.DialogueText::getstate(void*)", &MONO_DIALOGUETEXT::GetState);
		mono_add_internal_call("CSScript.DialogueText::setstate(void*,uint)", &MONO_DIALOGUETEXT::SetState);
		mono_add_internal_call("CSScript.DialogueText::getinitialscale(void*)", &MONO_DIALOGUETEXT::GetInitialScale);
		mono_add_internal_call("CSScript.DialogueText::setinitialscale(void*,single,single,single)", &MONO_DIALOGUETEXT::SetInitialScale);
		mono_add_internal_call("CSScript.DialogueText::getindex(void*)", &MONO_DIALOGUETEXT::GetIndex);
		mono_add_internal_call("CSScript.DialogueText::setindex(void*,uint)", &MONO_DIALOGUETEXT::SetIndex);
	}
}