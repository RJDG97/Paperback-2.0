#pragma once

#include "Systems/DebugSystem.h"

struct health_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "health_system"
    };

    struct NoHealthEvent : paperback::event::instance< entity&> {};
    struct UpdateHealthEvent : paperback::event::instance< entity&, entity&, health&> {};

    using query = std::tuple< 
        paperback::query::one_of<entity, healthbar, friendly, enemy>
    ,    paperback::query::none_of<prefab> 
    >;

    void operator()(paperback::component::entity& Entity, health& Health, healthbar& HealthBar, friendly& Friend, enemy& Enemy) noexcept
    {
        if (Entity.IsZombie()) return;

        if (Health.m_CurrentHealth <= 0) {
            BroadcastGlobalEvent<NoHealthEvent>(Entity);
            return;
        }

        if (&HealthBar) {
            // Initialize Query
            tools::query Query;

            Query.m_Must.AddFromComponents < health, base>();
            Query.m_OneOf.AddFromComponents < friendly, enemy>();
            Query.m_NoneOf.AddFromComponents< prefab >();

            ForEach(Search(Query), [&](paperback::component::entity& Dynamic_Entity, health& Dynamic_Health, base& Dyanmic_Base, friendly& Dynamic_Friend, enemy& Dynamic_Enemy)  noexcept
                {
                    if (Entity.IsZombie()) return;

                    // Do not check against self
                    if ((&Entity == &Dynamic_Entity) || (Dynamic_Entity.IsZombie())) return;

                    if (&Dyanmic_Base) {
                        if(&Friend && &Dynamic_Friend)
                            BroadcastGlobalEvent<UpdateHealthEvent>(Entity, Dynamic_Entity, Dynamic_Health);
                        else if(&Enemy && &Dynamic_Enemy)
                            BroadcastGlobalEvent<UpdateHealthEvent>(Entity, Dynamic_Entity, Dynamic_Health);
                    }
                });
        }
    }
};