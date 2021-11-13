#pragma once

struct enemy_spawner_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "timer_system"
	};

    using query = std::tuple
    <
        paperback::query::must<transform, timer, counter, enemy_spawner>
    ,   paperback::query::none_of<prefab, reference_prefab>
    >;

    tools::query m_ActiveSpawner;
    tools::query m_ActiveEnemy;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<Input::KeyPressed>( this );
        RegisterGlobalEventClass<Input::MousePressed>( this );
        
        m_ActiveSpawner.m_Must.AddFromComponents<transform, timer, enemy_spawner, counter>();
        m_ActiveSpawner.m_NoneOf.AddFromComponents<prefab, reference_prefab>();

        // Enemy Prefab
        m_ActiveEnemy.m_Must.AddFromComponents<prefab, transform, waypoint, waypoint_tag, mesh, animator, health, damage, enemy>();
        m_ActiveEnemy.m_NoneOf.AddFromComponents<counter>();
    }

    // Enemy Spawner
	void operator()( entity& Entity, transform& Transform, timer& Timer, counter& Counter, enemy_spawner& Spawner, waypoint_tag& STag ) noexcept
	{
        if (Counter.m_Value > 0 && Timer.m_Value <= 0.0f)
        {
            if (Spawner.m_EnemyPrefabGuid == 0) return;

            // Reset timer
            Timer.m_Value = Timer.m_Cooldown;


            // Find Enemy Prefab Details (Archetype& and GID)
            auto& EnemyPrefab   = m_Coordinator.GetArchetype( Spawner.m_EnemyPrefabGuid );
            auto EnemyPrefabGID = EnemyPrefab.FindPrefabEntityGID( Spawner.m_PrefabType );

            ////// Find Enemy Prefab Details (Archetype& and GID)
            //auto Enemies = Search( m_ActiveEnemy );
            //if (Enemies.size() != 1 ) return;
            //auto& Enemy = *( Enemies[0] );
            //auto EnemyPrefabGID = Enemy.FindPrefabEntityGID( Spawner.m_PrefabType );

            if ( EnemyPrefabGID == paperback::settings::invalid_index_v ) return;

            // Find Enemy Prefab Info and Clone
            auto PrefabInfo = GetEntityInfo( EnemyPrefabGID );
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
        //    Counter.m_Value = 1;
        //    Timer.m_Value = Timer.m_Cooldown;
        //}
	}

    // Updates enemy spawner count
	void OnEvent( const size_t& Key ) noexcept
    {
        // This check is to be replaced with Player Controller
        if ( Key == GLFW_KEY_4 )
        {
            // Player Info Loop
		    ForEach( Search( m_ActiveSpawner ), [&]( entity& Entity, timer& Timer, counter& Counter ) noexcept
            {
                if ( Counter.m_Value <= 0 )
                {
                    Timer.m_Value = Timer.m_Cooldown;
                    Counter.m_Value = 1;
                }
            });
        }
    }
};