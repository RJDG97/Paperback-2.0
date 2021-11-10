#pragma once

struct player_spawner_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "timer_system"
	};

    using query = std::tuple
    <
        paperback::query::must<transform, friendly_spawner>
    ,   paperback::query::none_of<reference_prefab>
    >;

    //tools::query m_QueryPlayerSpawner;
    tools::query m_Player;
    tools::query m_PlayerPrefabArchetype;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<Input::KeyPressed>( this );
        RegisterGlobalEventClass<Input::MousePressed>( this );

        //// Entity Spawner
        //m_QueryPlayerSpawner.m_Must.AddFromComponents<transform, waypoint, friendly_spawner>();
        //m_QueryPlayerSpawner.m_NoneOf.AddFromComponents<reference_prefab, prefab, counter>();

        // Actual Player
        /*
            entity, transform, reference_prefab, cost (Money)
        */
        m_Player.m_Must.AddFromComponents<transform, reference_prefab, cost>();
        //m_QueryPlayerSpawner.m_NoneOf.AddFromComponents<reference_prefab, prefab, counter>();

        // Friendly Prefab
        m_PlayerPrefabArchetype.m_Must.AddFromComponents<prefab, transform, waypoint, mesh, animator, health, damage, friendly>();
        m_PlayerPrefabArchetype.m_NoneOf.AddFromComponents<counter>();
    }

    // Note this Entity should not be a Prefab Instance (Because of Add/Remove)
    // The spawning should be in an Event? On buttom press or sth
	void operator()( entity& Entity, transform& Transform, counter& Counter, waypoint& Waypoint ) noexcept
	{
        //if ( Timer.m_Value <= 0.0f && Counter.m_Value > 0 )
        //{
        //    // Reset timer
        //    Timer.m_Value = 0.2f;

        //    // Searches for Player Unit Archetype
        //    auto Friendlies = Search( m_PlayerPrefabArchetype );
        //    if ( Friendlies.size() != 1 ) return;
        //    // Grab Archetype&
        //    auto& Friendly = *( Friendlies[0] );

        //    // Temporary, Clone Prefab 0 in Archetype
        //    auto  ClonedGID  = Friendly.ClonePrefab( 0 ); // Hardcoded part for Prefab Entity Index
        //    auto& ClonedInfo = GetEntityInfo( ClonedGID );

        //    // Update values of new friendly
        //    auto [CTransform, CWaypoint] = Friendly.GetComponents<transform, waypoint>( ClonedInfo.m_PoolDetails );
        //    CTransform.m_Position = Transform.m_Position;
        //    CWaypoint.m_Value = Waypoint.m_Value;
        //}

        //if ( --Counter.m_Value <= 0 )
        //        m_Coordinator.AddOrRemoveComponents<std::tuple<>, std::tuple<counter>>( Entity );
	}

	void OnEvent( int Key ) noexcept
    {
        if ( Key == GLFW_KEY_SPACE )
        {
      //      // Grab Player Unit Prefab
      //      auto Friendlies = Search( m_PlayerPrefabArchetype );
      //      if ( Friendlies.size() != 1 ) return;
      //      auto& Friendly = *( Friendlies[0] );

		    //ForEach( Search( m_Player ), [&]( const entity& Entity, cost& Money ) noexcept
      //      {
      //          // Name should be passed by collision entity or sth (If card)
      //          auto PrefabGID = Friendly.FindPrefabEntityGID( "Normal" );
      //          if ( PrefabGID == paperback::settings::invalid_index_v ) return;
      //          auto PrefabInfo = GetEntityInfo( PrefabGID );

      //          auto [ CTransform, CWaypoint ] = PrefabInfo.m_pArchetype->GetComponents<transform, waypoint>( PrefabInfo.m_PoolDetails );
      //          //CTransform.m_Position = Transform.m_Position;
      //          //CWaypoint.m_Value = Waypoint.m_Value;
      //      });

            //ForEach( Search( m_QueryPlayerSpawner ), [&]( const entity& Entity, timer& Timer ) noexcept
            //{
            //    Timer.m_Value = 0.0f;
            //    // Add counter component to the spawner ( Deactivates this system until wave is over )
            //    m_Coordinator.AddOrRemoveComponents<std::tuple<counter>>( Entity, [&]( counter& Counter )
            //                  {
            //                      //Counter.m_Value = (( Wave.m_Value / 2 ) + 1 ) * 3;
            //                      //Counter.m_Value = Counter.m_Value / ( ( Wave.m_Value / 3 ) + 1 );
            //                      Counter.m_Value = 1;
            //                  });
            //});
        }
    }
};