#pragma once

struct enemy_spawner
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "EnemySpawner"
	};

    std::string    m_PrefabType;
    paperback::u64 m_EnemyPrefabGuid;
};

namespace RR_EnemySpawner
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<enemy_spawner>( enemy_spawner::typedef_v.m_pName )
            .constructor()( rttr::policy::ctor::as_object )
            .property( "Enemy Prefab Type", &enemy_spawner::m_PrefabType )( rttr::policy::prop::as_reference_wrapper )
            .property( "Enemy Prefab Guid", &enemy_spawner::m_EnemyPrefabGuid );
    }
}