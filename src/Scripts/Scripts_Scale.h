#pragma once

#include "Mono.h"

namespace MONO_SCALE
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_scale = m_obj.m_pArchetype->FindComponent<scale>(m_obj.m_PoolDetails);
		return m_scale;
	}

	MONO_EXPORT paperback::Vector3f GetValue(void* address)
	{
		if (address)
			return reinterpret_cast<scale*>(address)->m_Value;

		return {};
	}

	MONO_EXPORT void SetValue(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<scale*>(address)->m_Value = {x, y, z};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Scale::getaddress(uint)", &MONO_SCALE::GetAddress);
		mono_add_internal_call("CSScript.Scale::getvalue(void*)", &MONO_SCALE::GetValue);
		mono_add_internal_call("CSScript.Scale::setvalue(void*,single,single,single)", &MONO_SCALE::SetValue);
	}
}