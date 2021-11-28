#pragma once

struct card_script : paperback::script::card_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Card Script"
    };

    // You have to define this constructor - For internal registration
    card_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::card_interface{ Instance }
    { }

    // Override virtual functions defined within button_interface
    void OnHover(void) noexcept override
    {
        TRACE_PRINT("Button Hovered!");
    }

    void OnClick( paperback::u32 PrefabGID, int PositionIndex ) noexcept override
    {
        auto list = m_Coordinator.Search<paperback::component::entity>();
        TRACE_PRINT("Button Clicked!");
        TRACE_PRINT("Number Of Archetypes w/ Entity Component: " + std::to_string(list.size()));

        // BroadcastGlobalEvent<UnitSpawnEvent>(Card);

        //  {
                //// Check if GID is Valid
                //// Get Unit Info and Spawn unit
                //auto PrefabInfo = GetEntityInfo(Card->m_UnitGID);
                //if ( PrefabInfo.m_pArchetype )
                //{
                //    auto InstanceGID = PrefabInfo.m_pArchetype->ClonePrefab(PrefabInfo.m_PoolDetails.m_PoolIndex);
                //    // <Update Instance Info>
                //    // Call Get Object Pos Index
                //    int index = Card->m_index;
                //    // Delete Card
                //    DeleteEntity(obj);
                //    BroadcastGlobalEvent<CardSpawnEvent>( index );
                //}
        //  }

        // Spawn new Card

        // BroadcastGlobalEvent<CardSpawnEvent>(int);
       
        // {
                //// Initialize Query
                // tools::query Query;
                // 
                // Query.m_Must.AddFromComponents < deck>();
                // Query.m_NoneOf.AddFromComponents< prefab >();
                // 
                // ForEach(Search(Query), [&](paperback::component::entity& Dynamic_Entity, deck& Deck)  noexcept
                // {
                        // 
                        //bool CardsAvail = false;
                        //// Check if cards are available
                        //for (int i = 0; i < Deck->m_Deck.size(); i++) {
                        //    if (Deck->m_Deck[i].m_Count > 0)
                        //        CardsAvail = true;
                        //}
                        //// if no available cards
                        //if(!CardsAvail)
                        //    // Lose Game State

                        //while (CardsAvail) {
                        //    // Randomize card spawned
                        //    int cardindex = rand() % 3;
                        //    // If card is available
                        //    if (Deck->m_Deck[cardindex].m_Count > 0) {
                        //        // Decrease available card count
                        //        Deck->m_Deck[cardindex].m_Count--;

                        //        // Spawn Card
                        //        // Check if GID is Valid
                        //        if (Deck->m_Deck[cardindex].m_CardGID == paperback::settings::invalid_index_v) return; // Check Archetype* rather than GID, default value for uninitialized variables are prolly 0
                        //        // Get Unit Info and Spawn unit
                        //        auto PrefabInfo = GetEntityInfo(Deck->m_Deck[cardindex].m_CardGID);
                        //        auto InstanceGID = PrefabInfo.m_pArchetype->ClonePrefab(PrefabInfo.m_PoolDetails.m_PoolIndex);
                        //        // Update Card properties
                        //        auto m_obj = GetEntityInfo(InstanceGID);
                        //        // Update Position
                        //        transform* Transform = &m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
                        //        Transform->m_Position = Deck->m_Position[index];
                        //        // Update Card Index (position)
                        //        card* Card = &m_obj.m_pArchetype->GetComponent<card>(m_obj.m_PoolDetails);
                        //        Card->m_index = index;
                        //        break;
                        //    }
                        //}
                // }
        // }

        // Broadcast Event For New Card After Using Old Card
        // BroadcastGlobalEvent<UnitSpawnEvent>(Card);
    }
};