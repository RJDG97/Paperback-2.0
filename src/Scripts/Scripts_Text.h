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

	MONO_EXPORT std::string GetFont(void* address)
	{
		if (address)
			return reinterpret_cast<text*>(address)->m_Font;

		return {};
	}

	MONO_EXPORT std::string GetText(void* address)
	{
		if (address)
			return reinterpret_cast<text*>(address)->m_Text;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetColor(void* address)
	{
		if (address)
			return reinterpret_cast<text*>(address)->m_Color;

		return {};
	}

	MONO_EXPORT void SetFont(void* address, std::string value)
	{
		if (address)
			reinterpret_cast<text*>(address)->m_Font = value;
	}

	MONO_EXPORT void SetText(void* address, std::string value)
	{
		if (address)
			reinterpret_cast<text*>(address)->m_Text = value;
	}

	MONO_EXPORT void SetColor(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<text*>(address)->m_Color = {x, y ,z};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Text::getaddress(uint)", &MONO_TEXT::GetAddress);
		mono_add_internal_call("CSScript.Text::getfont(void*)", &MONO_TEXT::GetFont);
		mono_add_internal_call("CSScript.Text::setfont(void*,string)", &MONO_TEXT::SetFont);
		mono_add_internal_call("CSScript.Text::gettext(void*)", &MONO_TEXT::GetText);
		mono_add_internal_call("CSScript.Text::settext(void*,string)", &MONO_TEXT::SetText);
		mono_add_internal_call("CSScript.Text::getcolor(void*)", &MONO_TEXT::GetColor);
		mono_add_internal_call("CSScript.Text::setcolor(void*,single,single,single)", &MONO_TEXT::SetColor);
	}
}