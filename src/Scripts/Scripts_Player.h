#pragma once

#include "Mono.h"
#include "paperback_camera.h"

namespace MONO_PLAYER
{

	MONO_EXPORT int32_t GetJumpUnitID()
	{
		int32_t GID{ -1 };
		tools::query Query;
		Query.m_Must.AddFromComponents<player_controller, name, paperback::component::entity>();

		PPB.ForEach(PPB.Search(Query), [&](player_controller& PlayerController, name& Name, paperback::component::entity& Entity) noexcept -> bool
		{
			if (Name.m_Value == "Jump Unit")
			{
				GID = Entity.m_GlobalIndex;
				return true;
			}

			return false;
		});

		return GID;
	}

	MONO_EXPORT int32_t GetPushUnitID()
	{
		int32_t GID{ -1 };
		tools::query Query;
		Query.m_Must.AddFromComponents<player_controller, name, paperback::component::entity>();

		PPB.ForEach(PPB.Search(Query), [&](player_controller& PlayerController, name& Name, paperback::component::entity& Entity) noexcept -> bool
		{
			if (Name.m_Value == "Push Unit")
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
		// no longer handled here
		//PPB.TogglePlayers();
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Player::GetJumpUnitID()", &MONO_PLAYER::GetJumpUnitID);
		mono_add_internal_call("CSScript.Player::GetPushUnitID()", &MONO_PLAYER::GetPushUnitID);
		mono_add_internal_call("CSScript.Player::TogglePlayers()", &MONO_PLAYER::TogglePlayers);
	}
}