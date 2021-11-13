#pragma once

struct player_spawner_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "PlayerSpawnerSystem"
	};

    using query = std::tuple
    <
        paperback::query::must<transform, timer, counter, friendly_spawner>
    ,   paperback::query::none_of<prefab, reference_prefab, enemy_spawner>
    >;

    tools::query m_QueryPlayer;
    tools::query m_ActiveSpawner;
    tools::query m_ActiveFriendly;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<Input::KeyPressed>( this );
        RegisterGlobalEventClass<Input::MousePressed>( this );
        
        // Query for player
        m_QueryPlayer.m_Must.AddFromComponents<reference_prefab, currency, player>();
        m_QueryPlayer.m_NoneOf.AddFromComponents<prefab>();

        // Query for spawner
        m_ActiveSpawner.m_Must.AddFromComponents<transform, timer, friendly_spawner, counter>();
        m_ActiveSpawner.m_NoneOf.AddFromComponents<prefab, reference_prefab>();

        // Friendly Prefab
        m_ActiveFriendly.m_Must.AddFromComponents<prefab, transform, waypoint, waypoint_tag, mesh, animator, health, damage, friendly>();
        m_ActiveFriendly.m_NoneOf.AddFromComponents<counter>();
    }

    // Enemy Spawner
	void operator()( entity& Entity, transform& Transform, timer& Timer, counter& Counter, friendly_spawner& Spawner, waypoint_tag& STag ) noexcept
	{
        if (Counter.m_Value > 0 && Timer.m_Value <= 0.0f)
        {
            if (Spawner.m_FriendlyPrefabGuid == 0) return;

            // Find Enemy Prefab Details (Archetype& and GID)
            auto& FriendlyPrefab   = m_Coordinator.GetArchetype( Spawner.m_FriendlyPrefabGuid );
            auto FriendlyPrefabGID = FriendlyPrefab.FindPrefabEntityGID( Spawner.m_PrefabType );

            ////// Find Enemy Prefab Details (Archetype& and GID)                         - Just store friendly tagged unit guid
            //auto Enemies = Search( m_ActiveFriendly );
            //if (Enemies.size() != 1 ) return;
            //auto& Enemy = *( Enemies[0] );
            //auto EnemyPrefabGID = Enemy.FindPrefabEntityGID( Spawner.m_PrefabType );

            if ( FriendlyPrefabGID == paperback::settings::invalid_index_v ) return;

            // Find Enemy Prefab Info and Clone
            auto PrefabInfo = GetEntityInfo( FriendlyPrefabGID );
            auto InstanceGID = PrefabInfo.m_pArchetype->ClonePrefab( PrefabInfo.m_PoolDetails.m_PoolIndex );

            // Update Components
            auto& IInfo = GetEntityInfo( InstanceGID );
            auto [ITransform, ITag, IAnimator] = IInfo.m_pArchetype->GetComponents<transform, waypoint_tag, animator>( IInfo.m_PoolDetails );

            ITransform.m_Position = Transform.m_Position;
            ITag.m_Value = STag.m_Value;
            IAnimator.m_PlayOnce = false;

            --Counter.m_Value;
        }

        //if ( Counter.m_Value == 0 )
        //{
        //    Counter.m_Value = 3;
        //    Timer.m_Value = 10.0f;
        //    Timer.m_Cooldown = 2.0f;
        //}
	}

    // Updates player spawner count
	void OnEvent( const size_t& Key ) noexcept
    {
        // This check is to be replaced with Player Controller
        if ( Key == GLFW_KEY_1 || Key == GLFW_KEY_2 || Key == GLFW_KEY_3 )
        {
            // Player Info Loop
		    ForEach( Search( m_ActiveSpawner ), [&]( entity& Entity, timer& Timer, counter& Counter, waypoint_tag& Tag ) noexcept
            {
                if ( !( Key == GLFW_KEY_1 && Tag.m_Value == 0 ) &&
                     !( Key == GLFW_KEY_2 && Tag.m_Value == 1 ) &&
                     !( Key == GLFW_KEY_3 && Tag.m_Value == 2 ) )
                    return;

                Timer.m_Value = 0.0f;
                Counter.m_Value = 1;
            });
        }
    }
};