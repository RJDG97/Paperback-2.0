#pragma once

struct enemy_spawner
{
	constexpr static auto typedef_v = paperback::component::type::tag
	{
		.m_pName = "EnemySpawner"
	};
};

namespace RR_EnemySpawner
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<enemy_spawner>( enemy_spawner::typedef_v.m_pName )
            .constructor()( rttr::policy::ctor::as_object );
    }
}