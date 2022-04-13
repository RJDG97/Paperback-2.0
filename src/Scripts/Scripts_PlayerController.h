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
	MONO_EXPORT MonoArray* GetAbilities(void* address)
	{
		if (address)
		{
			MonoArray* temp_array = mono_array_new(mono_domain_get(), mono_get_uint32_class(), reinterpret_cast<player_controller*>(address)->m_Abilities.size());

			for (size_t i = 0; i != reinterpret_cast<player_controller*>(address)->m_Abilities.size(); ++i)
			{
				mono_array_set(temp_array, int32_t, i, reinterpret_cast<player_controller*>(address)->m_Abilities[i]);
			}

			return temp_array;
		}

		return {};
	}

	MONO_EXPORT void AddAbility(void* address, int32_t ability)
	{
		if (address)
			reinterpret_cast<player_controller*>(address)->m_Abilities.push_back(static_cast<player_controller::Ability>(ability));
	}

	MONO_EXPORT void RotateAbility(void* address)
	{
		if (address)
		{
			auto& abilities { reinterpret_cast<player_controller*>(address)->m_Abilities };

			if (abilities.size() > 1)
			{
				player_controller::Ability first_ability = *(abilities.begin());
				abilities.erase(abilities.begin());
				abilities.push_back(first_ability);
			}
		}
	}

	MONO_EXPORT int GetCheckpointID(void* address)
	{
		if (address)
			return reinterpret_cast<player_controller*>(address)->m_CheckpointID;

		return false;
	}

	MONO_EXPORT void SetCheckpointID(void* address, int value)
	{
		if (address)
			reinterpret_cast<player_controller*>(address)->m_CheckpointID = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.PlayerController::getaddress(uint)", &MONO_PLAYERCONTROLLER::GetAddress);
		mono_add_internal_call("CSScript.PlayerController::GetFPSMode(void*)", &MONO_PLAYERCONTROLLER::GetFPSMode);
		mono_add_internal_call("CSScript.PlayerController::SetFPSMode(void*,bool)", &MONO_PLAYERCONTROLLER::SetFPSMode);

		// Player Grow
		mono_add_internal_call("CSScript.PlayerController::GetAbilities(void*)", &MONO_PLAYERCONTROLLER::GetAbilities);
		mono_add_internal_call("CSScript.PlayerController::Add_Ability(void*,int)", &MONO_PLAYERCONTROLLER::AddAbility);
		mono_add_internal_call("CSScript.PlayerController::Rotate_Abilities(void*)", &MONO_PLAYERCONTROLLER::RotateAbility);

		mono_add_internal_call("CSScript.PlayerController::GetCheckpointID(void*)", &MONO_PLAYERCONTROLLER::GetCheckpointID);
		mono_add_internal_call("CSScript.PlayerController::SetCheckpointID(void*,int)", &MONO_PLAYERCONTROLLER::SetCheckpointID);
	}
}