#pragma once

#include "Systems/DebugSystem.h"

struct deck_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "Deck_system"
    };

    using query = std::tuple<
        paperback::query::must<entity, deck>
        , paperback::query::none_of<prefab>
    >;

    void operator()(paperback::component::entity& Entity, deck& Deck) noexcept
    {
        if (Entity.IsZombie()) return;
    }
};