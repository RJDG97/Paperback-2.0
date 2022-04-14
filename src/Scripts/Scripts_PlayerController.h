/**********************************************************************************
*\file         Scripts_PlayerController.h
*\brief        Scripts_PlayerController.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

	MONO_EXPORT bool GetFPSMode(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_FPSMode;

		return {};
	}

	MONO_EXPORT void SetFPSMode(uint32_t ID, bool value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_FPSMode = value;
	}

	// Player Grow
	MONO_EXPORT bool GetGrowStatus(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_GrowAvailable;

		return false;
	}

	MONO_EXPORT void SetGrowStatus(uint32_t ID, bool value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_GrowAvailable = value;
	}


	// Player Shrink
	MONO_EXPORT bool GetShrinkStatus(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_ShrinkAvailable;

		return false;
	}

	MONO_EXPORT void SetShrinkStatus(uint32_t ID, bool value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_ShrinkAvailable = value;
	}


	// Player Freeze
	MONO_EXPORT bool GetFreezeStatus(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_FreezeAvailable;

		return false;
	}

	MONO_EXPORT void SetFreezeStatus(uint32_t ID, bool value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_FreezeAvailable = value;
	}

	MONO_EXPORT int GetCheckpointID(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_CheckpointID;

		return false;
	}

	MONO_EXPORT void SetCheckpointID(uint32_t ID, int value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<player_controller>(m_obj.m_PoolDetails)->m_CheckpointID = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.PlayerController::getaddress(uint)", &MONO_PLAYERCONTROLLER::GetAddress);
		mono_add_internal_call("CSScript.PlayerController::GetFPSMode(uint)", &MONO_PLAYERCONTROLLER::GetFPSMode);
		mono_add_internal_call("CSScript.PlayerController::SetFPSMode(uint,bool)", &MONO_PLAYERCONTROLLER::SetFPSMode);

		// Player Grow
		mono_add_internal_call("CSScript.PlayerController::GetGrowStatus(uint)", &MONO_PLAYERCONTROLLER::GetGrowStatus);
		mono_add_internal_call("CSScript.PlayerController::SetGrowStatus(uint,bool)", &MONO_PLAYERCONTROLLER::SetGrowStatus);

		// Player Shrink
		mono_add_internal_call("CSScript.PlayerController::GetShrinkStatus(uint)", &MONO_PLAYERCONTROLLER::GetShrinkStatus);
		mono_add_internal_call("CSScript.PlayerController::SetShrinkStatus(uint,bool)", &MONO_PLAYERCONTROLLER::SetShrinkStatus);

		// Player Freeze
		mono_add_internal_call("CSScript.PlayerController::GetFreezeStatus(uint)", &MONO_PLAYERCONTROLLER::GetFreezeStatus);
		mono_add_internal_call("CSScript.PlayerController::SetFreezeStatus(uint,bool)", &MONO_PLAYERCONTROLLER::SetFreezeStatus);

		mono_add_internal_call("CSScript.PlayerController::GetCheckpointID(uint)", &MONO_PLAYERCONTROLLER::GetCheckpointID);
		mono_add_internal_call("CSScript.PlayerController::SetCheckpointID(uint,int)", &MONO_PLAYERCONTROLLER::SetCheckpointID);
	}
}