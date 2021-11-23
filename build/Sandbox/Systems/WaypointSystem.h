#pragma once

struct waypoint_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "Waypoint System"
    };

    using query = std::tuple
    <
        paperback::query::must<transform, waypoint, waypoint_tag>
    ,   paperback::query::one_of<friendly, enemy>
    ,   paperback::query::none_of<prefab, rigidforce, rigidbody>
    >;

    tools::query m_UnitQuery;
    static constexpr auto Destination = 100;

    PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
        m_UnitQuery.m_Must.AddFromComponents<transform, rigidforce, rigidbody, waypoint, waypoint_tag>();
        m_UnitQuery.m_OneOf.AddFromComponents<friendly, enemy>();
        m_UnitQuery.m_NoneOf.AddFromComponents<prefab>();
	}

    // Waypoint
    void operator()( entity& Entity, transform& WPos, waypoint& Waypoint, waypoint_tag& WPTag, friendly* F, enemy* E ) noexcept
    {
        // Units
        ForEach( Search( m_UnitQuery ), [&]( transform& UPos, rigidforce& RF, rigidbody& RB, waypoint& WP, waypoint_tag& UTag, friendly* Friendly, enemy* Enemy ) noexcept
        {
            // Both Spawner & Unit are same type
            // and Pathing Tags are the same
            if ( (WPTag.m_Value == UTag.m_Value) && 
                 ((F && Friendly) || (E && Enemy)) )
            {
                auto Dist = UPos.m_Position - WPos.m_Position;

                if ( constexpr auto distance_sq = 0.5f; Dist.MagnitudeSq() <= distance_sq )
                {
                    auto Direction             = Waypoint.m_Value - UPos.m_Position;
                    Direction                 /= Direction.Magnitude();

                    RF.m_Momentum              = Direction * 4.0f;
                    WP.m_Value                 = Waypoint.m_Value;
                }
            }
            // Friendly / Enemy Unit reached other base
            // Terminate forces
            else if ( ( F && Friendly && WPTag.m_Value == Destination ) ||
                      ( E && Enemy && WPTag.m_Value == Destination ) )
            {
                auto Dist = UPos.m_Position - WPos.m_Position;
                if ( constexpr auto distance_sq = 0.5f; Dist.MagnitudeSq() <= distance_sq )
                {
                    RF.m_Momentum              = {};
                    RF.m_Forces                = {};
                }
            }
        });
    }
};