#pragma once

struct onevent_LowDeckCount_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "OnEvent Point Captured System"
    };

    tools::query m_QueryCapturePtSpawner;

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<enemy_spawner_system::OnLowDeckCount>(this);

        m_QueryCapturePtSpawner.m_Must.AddFromComponents< capture_point, spawner >();
        m_QueryCapturePtSpawner.m_NoneOf.AddFromComponents< prefab, timer, friendly, enemy >();
    }

    void OnEvent( void ) noexcept
    {
        auto SpawnCapturePts = [&]
                               {
                                   ForEach( Search( m_QueryCapturePtSpawner ), [&]( spawner& Spawner, capture_point& CP, spawner_details& Details ) noexcept
                                   {
                                       for ( size_t i = 0, max = Spawner.m_Position.size(); i < max; ++i )
                                       {
                                           const auto& Info = GetEntityInfo( Details.m_GID );
                                           if ( !CP.m_Captured && Info.m_pArchetype )
                                           {
                                               const auto InstanceGID   = Info.m_pArchetype->ClonePrefab( Info.m_PoolDetails.m_PoolIndex );
                                               const auto& InstanceInfo = GetEntityInfo( InstanceGID );
                               
                                               if ( InstanceInfo.m_pArchetype )
                                               {
                                                   auto Transform = InstanceInfo.m_pArchetype->FindComponent<transform>( InstanceInfo.m_PoolDetails );
                                                   if ( Transform ) Transform->m_Position = Spawner.m_Position[i];
                                               }
                                           }
                                       }
                                       CP.m_Captured = true;
                                   });
                               };

        SpawnCapturePts();
    }
};