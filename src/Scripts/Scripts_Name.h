/**********************************************************************************
*\file         Scripts_Name.h
*\brief        Scripts_Name.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_NAME
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
		return m_name;
	}

	MONO_EXPORT MonoString* GetName(void* address)
	{
		if (address)
			return mono_string_new(mono_domain_get(), reinterpret_cast<name*>(address)->m_Value.c_str());

		return {};
	}

	MONO_EXPORT void SetName(void* address, MonoString* value)
	{
		if (address)
			reinterpret_cast<name*>(address)->m_Value = mono_string_to_utf8(value);
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Name::getaddress(uint)", &MONO_NAME::GetAddress);
		mono_add_internal_call("CSScript.Name::getname(void*)", &MONO_NAME::GetName);
		mono_add_internal_call("CSScript.Name::setname(void*,string)", &MONO_NAME::SetName);
	}
}