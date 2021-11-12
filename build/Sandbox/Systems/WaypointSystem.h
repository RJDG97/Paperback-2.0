#pragma once

struct waypoint_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "Waypoint System"
    };

    using query = std::tuple
    <
        paperback::query::must<transform, waypoint>
    ,   paperback::query::one_of<friendly, enemy>
    ,   paperback::query::none_of<prefab>
    >;

    tools::query m_UnitQuery;

    PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
        m_UnitQuery.m_Must.AddFromComponents<transform, rigidforce, rigidbody, waypoint>();
        m_UnitQuery.m_OneOf.AddFromComponents<friendly, enemy>();
        m_UnitQuery.m_NoneOf.AddFromComponents<prefab>();
	}

    void operator()( entity& Entity, transform& WPos, waypoint& Waypoint, friendly* F, enemy* E ) noexcept
    {
        ForEach( Search( m_UnitQuery ), [&]( transform& UPos, rigidforce& RF, rigidbody& RB, waypoint& WP, friendly* Friendly, enemy* Enemy ) noexcept
        {
            // Both Spawner & Unit are same type
            if ( (F && Friendly) || (E && Enemy) )
            {
                auto Dist = UPos.m_Position - WPos.m_Position;
                if ( constexpr auto distance_sq = 0.3f; Dist.MagnitudeSq() <= distance_sq )
                {
                    auto Direction             = Waypoint.m_Value - UPos.m_Position;
                    Direction                 /= Direction.Magnitude();

                    RF.m_Momentum              = Direction * 4.0f;
                    WP.m_Value                 = Waypoint.m_Value;
                }
            }
            else if ( (F && Enemy) || (E && Friendly) )
            {
                auto Dist = UPos.m_Position - WPos.m_Position;
                if ( constexpr auto distance_sq = 0.3f; Dist.MagnitudeSq() <= distance_sq )
                {
                    RF.m_Momentum              = {};
                    RF.m_Forces                = {};
                }
            }
        });
    }
};