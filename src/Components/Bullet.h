#pragma once

struct bullet
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "bullet"
    };

	paperback::component::entity m_Owner;
};