#pragma once

struct bullet_logic_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "bullet_logic_system"
    };

    void operator()( paperback::component::entity& Entity, transform& Transform, timer& Timer, bullet& Bullet ) noexcept
    {
        if (Entity.IsZombie()) return;

        Timer.m_Timer -= m_Coordinator.DeltaTime();
        if ( Timer.m_Timer <= 0.0f )
        {
            m_Coordinator.DeleteEntity(Entity);
            return;
        }

        // Check collisions
        tools::query Query;
        Query.m_Must.AddFromComponents<transform>();

        ForEach( Search( Query ), [&]( paperback::component::entity& Dynamic_Entity, transform& xform ) noexcept -> bool
        {
            assert(Entity.IsZombie() == false );

            // Do not check against self
            if ( ( &Entity == &Dynamic_Entity) || ( Dynamic_Entity.IsZombie() ) || ( Bullet.m_Owner.m_GlobalIndex == Dynamic_Entity.m_GlobalIndex ) ) return false;

            constexpr auto min_distance_v = 4;
            if ((Transform.m_Position - xform.m_Position).getLengthSquared() < min_distance_v * min_distance_v)
            {
                m_Coordinator.DeleteEntity( Entity );
                m_Coordinator.DeleteEntity( Dynamic_Entity );
                return true;
            }
            return false;
        });
    }
};