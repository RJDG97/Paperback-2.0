/**********************************************************************************
*\file         Scripts_DialogueText.h
*\brief        Scripts_DialogueText.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

	MONO_EXPORT MonoString* GetDialogueName(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return mono_string_new(mono_domain_get(), m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_DialogueName.c_str());

		return {};
	}

	MONO_EXPORT void SetDialogueName(uint32_t ID, MonoString* value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_DialogueName = mono_string_to_utf8(value);
	}

	MONO_EXPORT float GetElapsedTime(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_ElapsedTime;

		return {};
	}

	MONO_EXPORT void SetElapsedTime(uint32_t ID, float value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_ElapsedTime = value;
	}

	MONO_EXPORT uint32_t GetState(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_State;

		return {};
	}

	MONO_EXPORT void SetState(uint32_t ID, uint32_t value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_State = value;
	}

	MONO_EXPORT paperback::Vector3f GetInitialScale(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_InitialScale;

		return {};
	}

	MONO_EXPORT void SetInitialScale(uint32_t ID, float x, float y, float z)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_InitialScale = { x, y, z };
	}

	MONO_EXPORT uint32_t GetIndex(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_CurrentIndex;

		return {};
	}

	MONO_EXPORT void SetIndex(uint32_t ID, uint32_t value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<dialogue_text>(m_obj.m_PoolDetails)->m_CurrentIndex = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.DialogueText::getaddress(uint)", &MONO_DIALOGUETEXT::GetAddress);
		mono_add_internal_call("CSScript.DialogueText::getdialoguename(uint)", &MONO_DIALOGUETEXT::GetDialogueName);
		mono_add_internal_call("CSScript.DialogueText::setdialoguename(uint,string)", &MONO_DIALOGUETEXT::SetDialogueName);
		mono_add_internal_call("CSScript.DialogueText::getelapsedtime(uint)", &MONO_DIALOGUETEXT::GetElapsedTime);
		mono_add_internal_call("CSScript.DialogueText::setelapsedtime(uint,single)", &MONO_DIALOGUETEXT::SetElapsedTime);
		mono_add_internal_call("CSScript.DialogueText::getstate(uint)", &MONO_DIALOGUETEXT::GetState);
		mono_add_internal_call("CSScript.DialogueText::setstate(uint,uint)", &MONO_DIALOGUETEXT::SetState);
		mono_add_internal_call("CSScript.DialogueText::getinitialscale(uint)", &MONO_DIALOGUETEXT::GetInitialScale);
		mono_add_internal_call("CSScript.DialogueText::setinitialscale(uint,single,single,single)", &MONO_DIALOGUETEXT::SetInitialScale);
		mono_add_internal_call("CSScript.DialogueText::getindex(uint)", &MONO_DIALOGUETEXT::GetIndex);
		mono_add_internal_call("CSScript.DialogueText::setindex(uint,uint)", &MONO_DIALOGUETEXT::SetIndex);
	}
}