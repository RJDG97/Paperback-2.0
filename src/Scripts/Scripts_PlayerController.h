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

	// Player Grow
	MONO_EXPORT bool GetGrowStatus(void* address)
	{
		if (address)
			return reinterpret_cast<player_controller*>(address)->m_GrowAvailable;

		return false;
	}

	MONO_EXPORT void SetGrowStatus(void* address, bool value)
	{
		if (address)
			reinterpret_cast<player_controller*>(address)->m_GrowAvailable = value;
	}


	// Player Shrink
	MONO_EXPORT bool GetShrinkStatus(void* address)
	{
		if (address)
			return reinterpret_cast<player_controller*>(address)->m_ShrinkAvailable;

		return false;
	}

	MONO_EXPORT void SetShrinkStatus(void* address, bool value)
	{
		if (address)
			reinterpret_cast<player_controller*>(address)->m_ShrinkAvailable = value;
	}


	// Player Freeze
	MONO_EXPORT bool GetFreezeStatus(void* address)
	{
		if (address)
			return reinterpret_cast<player_controller*>(address)->m_FreezeAvailable;

		return false;
	}

	MONO_EXPORT void SetFreezeStatus(void* address, bool value)
	{
		if (address)
			reinterpret_cast<player_controller*>(address)->m_FreezeAvailable = value;
	}

	// Player Freeze
	MONO_EXPORT bool GetCheckpointID(void* address)
	{
		if (address)
			return reinterpret_cast<player_controller*>(address)->m_FreezeAvailable;

		return false;
	}

	MONO_EXPORT void SetCheckpointID(void* address, bool value)
	{
		if (address)
			reinterpret_cast<player_controller*>(address)->m_FreezeAvailable = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.PlayerController::getaddress(uint)", &MONO_PLAYERCONTROLLER::GetAddress);
		mono_add_internal_call("CSScript.PlayerController::GetFPSMode(void*)", &MONO_PLAYERCONTROLLER::GetFPSMode);
		mono_add_internal_call("CSScript.PlayerController::SetFPSMode(void*,bool)", &MONO_PLAYERCONTROLLER::SetFPSMode);

		// Player Grow
		mono_add_internal_call("CSScript.PlayerController::GetGrowStatus(void*)", &MONO_PLAYERCONTROLLER::GetGrowStatus);
		mono_add_internal_call("CSScript.PlayerController::SetGrowStatus(void*,bool)", &MONO_PLAYERCONTROLLER::SetGrowStatus);

		// Player Shrink
		mono_add_internal_call("CSScript.PlayerController::GetShrinkStatus(void*)", &MONO_PLAYERCONTROLLER::GetShrinkStatus);
		mono_add_internal_call("CSScript.PlayerController::SetShrinkStatus(void*,bool)", &MONO_PLAYERCONTROLLER::SetShrinkStatus);

		// Player Freeze
		mono_add_internal_call("CSScript.PlayerController::GetFreezeStatus(void*)", &MONO_PLAYERCONTROLLER::GetFreezeStatus);
		mono_add_internal_call("CSScript.PlayerController::SetFreezeStatus(void*,bool)", &MONO_PLAYERCONTROLLER::SetFreezeStatus);

		mono_add_internal_call("CSScript.PlayerController::GetCheckpointID(void*)", &MONO_PLAYERCONTROLLER::GetCheckpointID);
		mono_add_internal_call("CSScript.PlayerController::SetCheckpointID(void*,int)", &MONO_PLAYERCONTROLLER::SetCheckpointID);
	}
}