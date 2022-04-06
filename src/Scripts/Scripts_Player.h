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
		Query.m_NoneOf.AddFromComponents<prefab>();

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
		Query.m_NoneOf.AddFromComponents<prefab>();

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

	MONO_EXPORT int32_t GetDialogueTextID()
	{
		int32_t GID{ -1 };
		tools::query Query;
		Query.m_Must.AddFromComponents<dialogue_text, name, paperback::component::entity>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		PPB.ForEach(PPB.Search(Query), [&](dialogue_text& DialogueText, name& Name, paperback::component::entity& Entity) noexcept -> bool
		{
			if (Name.m_Value == "Dialogue Text")
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

	MONO_EXPORT MonoArray* GetAllFreezeable()
	{
		std::vector<uint32_t> ids{};
		tools::query Query;
		Query.m_Must.AddFromComponents<freezable, paperback::component::entity>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		PPB.ForEach(PPB.Search(Query), [&](freezable& Freezable, paperback::component::entity& Entity) noexcept
		{
			ids.push_back(Entity.m_GlobalIndex);
		});

		MonoArray* temp_array = mono_array_new(mono_domain_get(), mono_get_uint32_class(), ids.size());

		for (size_t i = 0; i != ids.size(); ++i)
		{
			mono_array_set(temp_array, uint32_t, i, ids[i]);
		}

		return temp_array;
	}

	MONO_EXPORT MonoArray* GetAllPushable()
	{
		std::vector<uint32_t> ids{};
		tools::query Query;
		Query.m_Must.AddFromComponents<pushable, paperback::component::entity>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		PPB.ForEach(PPB.Search(Query), [&](pushable& Pushable, paperback::component::entity& Entity) noexcept
			{
				ids.push_back(Entity.m_GlobalIndex);
			});

		MonoArray* temp_array = mono_array_new(mono_domain_get(), mono_get_uint32_class(), ids.size());

		for (size_t i = 0; i != ids.size(); ++i)
		{
			mono_array_set(temp_array, uint32_t, i, ids[i]);
		}

		return temp_array;
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Player::GetJumpUnitID()", &MONO_PLAYER::GetJumpUnitID);
		mono_add_internal_call("CSScript.Player::GetPushUnitID()", &MONO_PLAYER::GetPushUnitID);
		mono_add_internal_call("CSScript.Player::GetDialogueTextID()", &MONO_PLAYER::GetDialogueTextID);
		mono_add_internal_call("CSScript.Player::TogglePlayers()", &MONO_PLAYER::TogglePlayers);
		mono_add_internal_call("CSScript.Player::GetAllFreezeable()", &MONO_PLAYER::GetAllFreezeable);
		mono_add_internal_call("CSScript.Player::GetAllPushable()", &MONO_PLAYER::GetAllPushable);
	}
}