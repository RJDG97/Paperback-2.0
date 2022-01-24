#pragma once

#include "Mono.h"

namespace MONO_TEXT
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_text = m_obj.m_pArchetype->GetComponent<text>(m_obj.m_PoolDetails);
		return &m_text;
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