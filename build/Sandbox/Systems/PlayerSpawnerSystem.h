#pragma once

struct player_spawner_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "PlayerSpawnerSystem"
	};

    using query = std::tuple
    <
        paperback::query::must<transform, friendly_spawner>
    ,   paperback::query::none_of<reference_prefab>
    >;

    static constexpr auto UnitCost = 100;

    tools::query m_QueryPlayer;
    tools::query m_PlayerPrefabArchetype;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<Input::KeyPressed>( this );
        RegisterGlobalEventClass<Input::MousePressed>( this );

        // Actual Player
        m_QueryPlayer.m_Must.AddFromComponents<reference_prefab, currency, player>();
        m_QueryPlayer.m_NoneOf.AddFromComponents<prefab>();

        // Friendly Prefab
        m_PlayerPrefabArchetype.m_Must.AddFromComponents<prefab, transform, waypointUserv1, mesh, /*animator,*/ health, damage, friendly>();
        m_PlayerPrefabArchetype.m_NoneOf.AddFromComponents<counter>();
    }

    // Note this Entity should not be a Prefab Instance (Because of Add/Remove)
    // The spawning should be in an Event? On buttom press or sth
	void operator()( entity& Entity, transform& Transform, counter& Counter, waypoint& Waypoint ) noexcept
	{

	}

	void OnEvent( const size_t& Key ) noexcept
    {
        // This check is to be replaced with Player Controller
        if ( Key == GLFW_KEY_1 || Key == GLFW_KEY_2 || Key == GLFW_KEY_3 )
        {
            // Player Info Loop
		    ForEach( Search( m_QueryPlayer ), [&]( currency& PlayerCurrency ) noexcept
            {
                paperback::u32 PrefabGID = paperback::settings::invalid_index_v;

                // UnitCost should be inside of Spawner
                if ( PlayerCurrency.m_Value < UnitCost ) return;

                // Grab Player Unit Prefab
                auto Friendlies = Search( m_PlayerPrefabArchetype );
                if ( Friendlies.size() != 1 ) return;
                auto& Friendly = *( Friendlies[0] );

                // Get Info Of Prefab To Clone (Based on Key Pressed)
                if      ( Key == GLFW_KEY_1 ) PrefabGID = Friendly.FindPrefabEntityGID( "Top" );
                else if ( Key == GLFW_KEY_2 ) PrefabGID = Friendly.FindPrefabEntityGID( "Mid" );
                else if ( Key == GLFW_KEY_3 ) PrefabGID = Friendly.FindPrefabEntityGID( "Bot" );
                if ( PrefabGID == paperback::settings::invalid_index_v ) return;
                auto PrefabInfo = GetEntityInfo( PrefabGID );

                // Clone Prefab
                auto InstanceGID = Friendly.ClonePrefab( PrefabInfo.m_PoolDetails.m_PoolIndex );
                // Update Animator Component
                auto& IInfo = GetEntityInfo( InstanceGID );
                IInfo.m_pArchetype->GetComponent<animator>( IInfo.m_PoolDetails ).m_PlayOnce = false;

                // Update Player Currency
                PlayerCurrency.m_Value -= UnitCost;
            });
        }
    }
};