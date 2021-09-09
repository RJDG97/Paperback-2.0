#pragma once

struct ship_logic_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "ship_logic_system"
    };

    using query = std::tuple<
        paperback::query::none_of<bullet>
    >;

    void operator()( paperback::component::entity& Entity, transform& Transform, timer& Timer ) noexcept
    {
        if ( Timer.m_Timer > 0.0f )
        {
            Timer.m_Timer -= m_Coordinator.DeltaTime();
            return;
        }


        tools::query Query;
        Query.m_NoneOf.AddFromComponents<bullet>();

        ForEach( Search(Query), [&]( transform& xform ) noexcept -> bool
        {
            if (&Transform == &xform) return false;

            auto Direction = xform.m_Position - Transform.m_Position;
            const auto DistanceSq = Direction.getLengthSquared();

            constexpr auto min_distance_v = 30;

            if (DistanceSq < min_distance_v * min_distance_v)
            {
                Timer.m_Timer = 5;

                m_Engine.m_Coordinator.CreateEntity([&]( paperback::component::entity& Bullet_Entity, transform& New_Transform, rigidbody& New_RigidBody, timer& New_Timer, bullet& New_Bullet)
                {
                    UNREFERENCED_PARAMETER( Bullet_Entity );

                    Direction /= std::sqrt(DistanceSq);
                    New_RigidBody.m_Velocity = Direction * 80.0f;
                    New_Transform.m_Position = Transform.m_Position;

                    New_Bullet.m_Owner = Entity;
                    New_Timer.m_Timer = 3.0f;
                });

                return true;
            }
            return false;
        });
    }
};