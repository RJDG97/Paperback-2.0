#pragma once

#include "Mono.h"

namespace MONO_ROTATION
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_rotation = m_obj.m_pArchetype->FindComponent<rotation>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_rotation)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Rotation component." << std::endl;
		}
#endif

		return m_rotation;
	}

	MONO_EXPORT paperback::Vector3f GetValue(void* address)
	{
		if (address)
			return reinterpret_cast<rotation*>(address)->m_Value;

		return {};
	}

	MONO_EXPORT void SetValue(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<rotation*>(address)->m_Value = {x, y, z};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Rotation::getaddress(uint)", &MONO_ROTATION::GetAddress);
		mono_add_internal_call("CSScript.Rotation::getvalue(void*)", &MONO_ROTATION::GetValue);
		mono_add_internal_call("CSScript.Rotation::setvalue(void*,single,single,single)", &MONO_ROTATION::SetValue);
	}
}