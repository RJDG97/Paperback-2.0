/**********************************************************************************
*\file         Scripts_Text.h
*\brief        Scripts_Text.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_TEXT
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_text = m_obj.m_pArchetype->FindComponent<text>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_text)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Text component." << std::endl;
		}
#endif

		return m_text;
	}

	MONO_EXPORT std::string GetFont(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<text>(m_obj.m_PoolDetails)->m_Font;

		return {};
	}

	MONO_EXPORT std::string GetText(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<text>(m_obj.m_PoolDetails)->m_Text;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetColor(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<text>(m_obj.m_PoolDetails)->m_Color;

		return {};
	}

	MONO_EXPORT void SetFont(uint32_t ID, std::string value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<text>(m_obj.m_PoolDetails)->m_Font = value;
	}

	MONO_EXPORT void SetText(uint32_t ID, std::string value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<text>(m_obj.m_PoolDetails)->m_Text = value;
	}

	MONO_EXPORT void SetColor(uint32_t ID, float x, float y, float z)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<text>(m_obj.m_PoolDetails)->m_Color = {x, y ,z};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Text::getaddress(uint)", &MONO_TEXT::GetAddress);
		mono_add_internal_call("CSScript.Text::getfont(uint)", &MONO_TEXT::GetFont);
		mono_add_internal_call("CSScript.Text::setfont(uint,string)", &MONO_TEXT::SetFont);
		mono_add_internal_call("CSScript.Text::gettext(uint)", &MONO_TEXT::GetText);
		mono_add_internal_call("CSScript.Text::settext(uint,string)", &MONO_TEXT::SetText);
		mono_add_internal_call("CSScript.Text::getcolor(uint)", &MONO_TEXT::GetColor);
		mono_add_internal_call("CSScript.Text::setcolor(uint,single,single,single)", &MONO_TEXT::SetColor);
	}
}