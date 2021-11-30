#pragma once

struct enemy_spawner_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "timer_system"
	};

    using query = std::tuple
    <
        paperback::query::must<transform, timer, spawner>
    ,   paperback::query::none_of<prefab>
    >;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
    }

    // Enemy Spawner
	void operator()( entity& Entity, transform& Transform, timer& Timer, spawner& Spawner ) noexcept
	{
        if (Timer.m_Value <= 0.0f)
        {
            Timer.m_Cooldown = 8.0f;
            // Reset timer
            Timer.m_Value = Timer.m_Cooldown;

            // <Update Instance Info>
            tools::query Spawner_Query;

            Spawner_Query.m_Must.AddFromComponents < deck, enemy>();
            Spawner_Query.m_NoneOf.AddFromComponents< prefab >();

            m_Coordinator.ForEach(m_Coordinator.Search(Spawner_Query), [&](paperback::component::entity& Dynamic_Entity, deck& Dyanmic_Deck)  noexcept
                {
                    bool CardsAvail = false;
                    // Check if cards are available
                    for (int i = 0; i < Dyanmic_Deck.m_Deck.size(); i++) {
                        if (Dyanmic_Deck.m_Deck[i].m_Count > 0)
                            CardsAvail = true;
                    }
                    // if no available cards
                    //if (!CardsAvail)
                        // Win Game State

                    while (CardsAvail) {
                        // Randomize card spawned
                        int cardindex = rand() % 3;
                        // If card is available
                        if (Dyanmic_Deck.m_Deck[cardindex].m_Count > 0) {
                            // Decrease available card count
                            Dyanmic_Deck.m_Deck[cardindex].m_Count--;

                            // Spawn Card
                            // Check if GID is Valid
                            if (Dyanmic_Deck.m_Deck[cardindex].m_CardGID == paperback::settings::invalid_index_v) return; // Check Archetype* rather than GID, default value for uninitialized variables are prolly 0
                            // Get Unit Info and Spawn unit
                            auto PrefabInfo = m_Coordinator.GetEntityInfo(Dyanmic_Deck.m_Deck[cardindex].m_CardGID);
                            auto InstanceGID = PrefabInfo.m_pArchetype->ClonePrefab(PrefabInfo.m_PoolDetails.m_PoolIndex);
                            // Update Card properties
                            auto m_obj = m_Coordinator.GetEntityInfo(InstanceGID);
                            transform* Transform = &m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
                            path_follower* Path = &m_obj.m_pArchetype->GetComponent<path_follower>(m_obj.m_PoolDetails);
                            Spawner.lane = rand() % 3;
                            Transform->m_Position = Spawner.m_Position[Spawner.lane];
                            Path->m_ID = Spawner.lane;

                            break;
                        }
                    }
                });
        }
	}

    // Updates enemy spawner count
	void OnEvent( const size_t& Key, const bool& Clicked ) noexcept
    {
        // This check is to be replaced with Player Controller
      //  if ( Key == GLFW_KEY_4 && Clicked )
      //  {
      //      // Player Info Loop
		    //ForEach( Search( m_ActiveSpawner ), [&]( entity& Entity, timer& Timer, counter& Counter ) noexcept
      //      {
      //          if ( Counter.m_Value <= 0 )
      //          {
      //              Timer.m_Value = Timer.m_Cooldown;
      //              Counter.m_Value = 1;
      //          }
      //      });
      //  }
    }
};