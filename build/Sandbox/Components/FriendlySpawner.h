#pragma once

struct friendly_spawner
{
	constexpr static auto typedef_v = paperback::component::type::tag
	{
		.m_pName = "FriendlySpawner"
	};
};

namespace RR_FriendlySpawner
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<friendly_spawner>( friendly_spawner::typedef_v.m_pName )
            .constructor()( rttr::policy::ctor::as_object );
    }
}