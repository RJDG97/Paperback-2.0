#pragma once

struct onevent_PointCaptured_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "OnEvent Point Captured System"
    };

    tools::query m_FriendlyDeckQuery;
    tools::query m_EnemyDeckQuery;

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<onevent_UnitTriggerStay_system::PointCaptured>(this);

        m_FriendlyDeckQuery.m_Must.AddFromComponents < deck, friendly >();
        m_FriendlyDeckQuery.m_NoneOf.AddFromComponents < prefab >();

        m_EnemyDeckQuery.m_Must.AddFromComponents < deck, enemy >();
        m_EnemyDeckQuery.m_NoneOf.AddFromComponents < prefab >();
    }

    void OnEvent( const bool& Friendly ) noexcept
    {
        auto RefillDeck = [&]( const tools::query& Query )
                          {
                              ForEach( Search( Query ), [&]( deck& Deck ) noexcept
                              {
                                  for ( auto& Card : Deck.m_Deck )
                                      Card.m_Count = 10;
                              });
                          };

        if ( Friendly ) RefillDeck( m_FriendlyDeckQuery );
        else            RefillDeck( m_EnemyDeckQuery );
    }
};


struct onevent_CapturePointDamaged_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "OnEvent Capture Point Damage System"
    };

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<onevent_UnitTriggerStay_system::CapturePointDamaged>(this);
    }

    void OnEvent( entity& Entity, health& HP, const bool& Friendly ) noexcept
    {
        // Max Offset is 1.0f
        auto FriendlyRatio = HP.m_CurrentHealth < 0 
                           ? 1.0f
                           : HP.m_CurrentHealth > (HP.m_MaxHealth * 0.5f)
                             ? 0.0f
                             : ( (HP.m_MaxHealth * 0.5f) - HP.m_CurrentHealth ) / ( HP.m_MaxHealth * 0.5f );
        auto EnemyRatio    = HP.m_CurrentHealth > HP.m_MaxHealth
                           ? 1.0f 
                           : HP.m_CurrentHealth < (HP.m_MaxHealth * 0.5f)
                             ? 0.0f
                             : (  HP.m_CurrentHealth - (HP.m_MaxHealth * 0.5f)) / ( HP.m_MaxHealth * 0.5f );

        const auto& Info = GetEntityInfo( Entity.m_GlobalIndex );
        auto [Parent, Scale, Capture] = Info.m_pArchetype->FindComponents<parent, scale, capture_point>( Info.m_PoolDetails );

        if ( Parent && Scale && Capture && Parent->m_ChildrenGlobalIndexes.size() )
        {
            for ( auto it = Parent->m_ChildrenGlobalIndexes.begin(), end = Parent->m_ChildrenGlobalIndexes.end(); it != end; ++it )
            {
                const auto& CInfo = GetEntityInfo( *it );
                auto [COffset, CFriendly, CEnemy] = CInfo.m_pArchetype->FindComponents<offset, friendly, enemy>( CInfo.m_PoolDetails );

                if ( COffset && (CFriendly || CEnemy) )
                {
                    if      ( CFriendly ) COffset->m_PosOffset.y = /*(-Scale->m_Value.y * 0.5f) +*/ FriendlyRatio * 1.0f;
                    else if ( CEnemy )    COffset->m_PosOffset.y = /*(-Scale->m_Value.y * 0.5f) +*/ EnemyRatio * 1.0f;
                }
            }
        }
    }
};