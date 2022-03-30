#pragma once

#include "Mono.h"

namespace MONO_FREEZABLE
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_freezable = m_obj.m_pArchetype->FindComponent<freezable>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_freezable)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Freezable component." << std::endl;
		}
#endif

		return m_freezable;
	}

	MONO_EXPORT bool GetFrozen(void* address)
	{
		if (address)
			return reinterpret_cast<freezable*>(address)->m_Frozen;

		return {};
	}

	MONO_EXPORT void SetFrozen(void* address, bool value)
	{
		if (address)
			reinterpret_cast<freezable*>(address)->m_Frozen = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Freezable::getaddress(uint)", &MONO_FREEZABLE::GetAddress);
		mono_add_internal_call("CSScript.Freezable::getfrozen(void*)", &MONO_FREEZABLE::GetFrozen);
		mono_add_internal_call("CSScript.Freezable::setfrozen(void*,bool)", &MONO_FREEZABLE::SetFrozen);
	}
}