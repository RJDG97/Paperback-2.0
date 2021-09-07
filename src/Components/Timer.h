#pragma once

struct timer
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "timer"
    };

	float m_Timer = 0.0f;
};