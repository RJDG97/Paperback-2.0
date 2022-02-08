#pragma once

#include "Mono.h"

namespace MONO_PLAYERCONTROLLER
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_playercontroller = m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_playercontroller)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Player Controller component." << std::endl;
		}
#endif

		return m_playercontroller;
	}

	MONO_EXPORT bool GetFPSMode(void* address)
	{
		if (address)
			return reinterpret_cast<player_controller*>(address)->m_FPSMode;

		return {};
	}

	MONO_EXPORT void SetFPSMode(void* address, bool value)
	{
		if (address)
			reinterpret_cast<player_controller*>(address)->m_FPSMode = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.PlayerController::getaddress(uint)", &MONO_PLAYERCONTROLLER::GetAddress);
		mono_add_internal_call("CSScript.PlayerController::GetFPSMode(void*)", &MONO_PLAYERCONTROLLER::GetFPSMode);
		mono_add_internal_call("CSScript.PlayerController::SetFPSMode(void*,bool)", &MONO_PLAYERCONTROLLER::SetFPSMode);
	}
}