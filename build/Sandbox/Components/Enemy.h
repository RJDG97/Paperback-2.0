#pragma once

struct enemy
{
	constexpr static auto typedef_v = paperback::component::type::tag
	{
		.m_pName = "Enemy"
	};
};

namespace RR_Enemy
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<enemy>( enemy::typedef_v.m_pName )
            .constructor()( rttr::policy::ctor::as_object );
    }
}