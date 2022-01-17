#pragma once

struct enemy_spawner_system : paperback::system::pausable_instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "enemy_spawner_system"
	};

    using query = std::tuple
    <
        paperback::query::must<transform, timer, spawner>
    ,   paperback::query::none_of<prefab>
    >;

    // Data Members
    tools::query Spawner_Query;
    tools::query Text_Query;

    // Event
    struct OnLowDeckCount : paperback::event::instance<>{};

    PPB_FORCEINLINE
	void OnSystemCreated( void ) noexcept
	{
        Spawner_Query.m_Must.AddFromComponents < deck, enemy>();
        Spawner_Query.m_NoneOf.AddFromComponents< prefab >();

        Text_Query.m_Must.AddFromComponents < text, enemy>();
        Text_Query.m_NoneOf.AddFromComponents< prefab >();
	}

    // Enemy Spawner
	void operator()( entity& Entity, transform& Transform, timer& Timer, spawner& Spawner ) noexcept
	{
        if (Timer.m_Value <= 0.0f)
        {
            Timer.m_Cooldown = 8.0f;
            // Reset timer
            Timer.m_Value = Timer.m_Cooldown;
        
            m_Coordinator.ForEach(m_Coordinator.Search(Spawner_Query), [&](paperback::component::entity& Dynamic_Entity, deck& Dynamic_Deck, sound& Sound)  noexcept
                {
                    Sound.m_Trigger = true;
                    bool CardsAvail = false;
                    int deckno = 0;
                    // Check if cards are available
                    for (int i = 0; i < Dynamic_Deck.m_Deck.size(); i++) {
                        deckno += Dynamic_Deck.m_Deck[i].m_Count;
                        if (Dynamic_Deck.m_Deck[i].m_Count > 0)
                            CardsAvail = true;
                    }
                    // if no available cards
                    if (!CardsAvail) {
                        // Win Game State
                        PPB.QueueScene("GameWin");
                    }
                    else if ( deckno <= 15 )
                        m_Coordinator.BroadcastEvent<enemy_spawner_system::OnLowDeckCount>();
        
                    while (CardsAvail) {

                        // Randomize card spawned
                        int cardindex = rand() % 3;
                        // If card is available
                        if (Dynamic_Deck.m_Deck[cardindex].m_Count > 0) {
                            // Decrease available card count
                            Dynamic_Deck.m_Deck[cardindex].m_Count--;
                            deckno--;
        
                            m_Coordinator.ForEach(m_Coordinator.Search(Text_Query), [&](paperback::component::entity& Dynamic_Entity, text& Text)  noexcept
                                {
                                    Text.m_Text = std::to_string(deckno);
                                });
        
                            // Spawn Card
                            // Check if GID is Valid
                            if (Dynamic_Deck.m_Deck[cardindex].m_CardGID == paperback::settings::invalid_index_v) return;
                            // Get Unit Info and Spawn unit
                            auto PrefabInfo = m_Coordinator.GetEntityInfo(Dynamic_Deck.m_Deck[cardindex].m_CardGID);
        
                            if ( PrefabInfo.m_pArchetype )
                            {
                                auto InstanceGID = PrefabInfo.m_pArchetype->ClonePrefab(PrefabInfo.m_PoolDetails.m_PoolIndex);
                                // Update Card properties
                                auto m_obj = m_Coordinator.GetEntityInfo(InstanceGID);
                                transform* Transform = &m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
                                path_follower* Path = &m_obj.m_pArchetype->GetComponent<path_follower>(m_obj.m_PoolDetails);
                                Spawner.lane = rand() % 3;
                                Transform->m_Position = Spawner.m_Position[Spawner.lane];
                                Path->m_ID = Spawner.lane;
        
        

                                // Add Unit To Hash Grid - After Position Update
                                auto [Entity, Xform, Box, Prefab] = m_obj.m_pArchetype->FindComponents<paperback::component::entity, transform, boundingbox, prefab>( m_obj.m_PoolDetails );
                                if ( !Prefab && Entity && Xform && Box )
                                    m_Coordinator.UpdateUnit( Entity->m_GlobalIndex, {0.0f,0.0f,0.0f}, Xform->m_Position, Box->Min, Box->Max );
                            }
        
                            break;
                        }
                    }
                });
        }
	}
};