#pragma once

#include "Mono.h"
#include "paperback_camera.h"

namespace MONO_PLAYER
{

	MONO_EXPORT int32_t GetRedRobotID()
	{
		int32_t GID{ -1 };
		tools::query Query;
		Query.m_Must.AddFromComponents<player_controller, name, paperback::component::entity>();

		PPB.ForEach(PPB.Search(Query), [&](player_controller& PlayerController, name& Name, paperback::component::entity& Entity) noexcept -> bool
		{
			if (Name.m_Value == "RedRobot")
			{
				GID = Entity.m_GlobalIndex;
				return true;
			}

			return false;
		});

		return GID;
	}

	MONO_EXPORT int32_t GetBlueRobotID()
	{
		int32_t GID{ -1 };
		tools::query Query;
		Query.m_Must.AddFromComponents<player_controller, name, paperback::component::entity>();

		PPB.ForEach(PPB.Search(Query), [&](player_controller& PlayerController, name& Name, paperback::component::entity& Entity) noexcept -> bool
		{
			if (Name.m_Value == "BlueRobot")
			{
				GID = Entity.m_GlobalIndex;
				return true;
			}

			return false;
		});

		return GID;
	}


	MONO_EXPORT void TogglePlayers()
	{
		PPB.TogglePlayers();
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Player::GetRedRobotID()", &MONO_PLAYER::GetRedRobotID);
		mono_add_internal_call("CSScript.Player::GetBlueRobotID()", &MONO_PLAYER::GetBlueRobotID);
		mono_add_internal_call("CSScript.Player::TogglePlayers()", &MONO_PLAYER::TogglePlayers);
	}
}