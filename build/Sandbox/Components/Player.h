#pragma once

struct player
{
	constexpr static auto typedef_v = paperback::component::type::tag
	{
		.m_pName = "Player"
	};
};

namespace RR_Player
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<player>( player::typedef_v.m_pName )
            .constructor()( rttr::policy::ctor::as_object );
    }
}