#pragma once

#include "Mono.h"
#include "paperback_camera.h"

namespace MONO_PLAYER
{

	MONO_EXPORT uint32_t GetRedRobotID()
	{
		//tools::query Query;
		//Query.m_Must.AddFromComponents<player_controller, name, paperback::component::entity>();

		//ForEach(Search(Query), [&](player_controller& PlayerController, name& Name, paperback::component::entity& Entity) noexcept
		//{
		//	if (Name.m_value == "Red") //might change check for something else
		//	{
		//		return Entity.m_GlobalIndex;
		//	}
		//});

		return {};
	}

	MONO_EXPORT uint32_t GetBlueRobotID()
	{
		//tools::query Query;
		//Query.m_Must.AddFromComponents<player_controller, name, paperback::component::entity>();

		//ForEach(Search(Query), [&](player_controller& PlayerController, name& Name, paperback::component::entity& Entity) noexcept
		//{
		//	if (Name.m_value == "Blue") //might change check for something else
		//	{
		//		return Entity.m_GlobalIndex;
		//	}
		//});

		return {};
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Player::GetRedRobotID()", &MONO_PLAYER::GetRedRobotID);
		mono_add_internal_call("CSScript.Player::GetBlueRobotID()", &MONO_PLAYER::GetBlueRobotID);
	}
}