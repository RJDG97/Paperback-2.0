#pragma once

#include "Mono.h"

namespace MONO_NAME
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_name = m_obj.m_pArchetype->GetComponent<name>(m_obj.m_PoolDetails);
		return &m_name;
	}

	MONO_EXPORT std::string GetName(void* address)
	{
		if (address)
			return reinterpret_cast<name*>(address)->m_Value;

		return {};
	}

	MONO_EXPORT void SetName(void* address, std::string value)
	{
		if (address)
			reinterpret_cast<name*>(address)->m_Value = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Mesh::getaddress(uint)", &MONO_NAME::GetAddress);
		mono_add_internal_call("CSScript.Mesh::getname(void*)", &MONO_NAME::GetName);
		mono_add_internal_call("CSScript.Mesh::setname(void*,string)", &MONO_NAME::SetName);
	}
}