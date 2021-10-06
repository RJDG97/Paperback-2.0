#pragma once

struct player
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Player"
    };

	bool m_IsPlayer;
};


namespace RR_Player
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<player>(player::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("Player", &player::m_IsPlayer);
    }
}