#pragma once

struct friendly_spawner
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "FriendlySpawner"
	};

    std::string    m_PrefabType;
    paperback::u64 m_FriendlyPrefabGuid;
};

namespace RR_FriendlySpawner
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<friendly_spawner>( friendly_spawner::typedef_v.m_pName )
            .constructor()( rttr::policy::ctor::as_object )
            .property( "Friendly Prefab Type", &friendly_spawner::m_PrefabType )( rttr::policy::prop::as_reference_wrapper )
            .property( "Friendly Prefab Guid", &friendly_spawner::m_FriendlyPrefabGuid );
    }
}