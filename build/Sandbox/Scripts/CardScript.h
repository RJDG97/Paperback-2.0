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
    }

    void OnClick(paperback::component::entity& Entity, paperback::u32 PrefabGID, int PositionIndex ) noexcept override
    {
        auto list = m_Coordinator.Search<paperback::component::entity>();

        // Check if GID is Valid
        // Get Unit Info and Spawn unit
        auto PrefabInfo = m_Coordinator.GetEntityInfo(PrefabGID);
        if ( PrefabInfo.m_pArchetype )
        {
            auto InstanceGID = PrefabInfo.m_pArchetype->ClonePrefab(PrefabInfo.m_PoolDetails.m_PoolIndex);

            // <Update Instance Info>
        }

        // Spawn new Card
       
        // Initialize Query
        tools::query Query;
        
        Query.m_Must.AddFromComponents < deck>();
        Query.m_NoneOf.AddFromComponents< prefab >();
        
        m_Coordinator.ForEach(m_Coordinator.Search(Query), [&](paperback::component::entity& Dynamic_Entity, deck& Deck)  noexcept
            {

                bool CardsAvail = false;
                // Check if cards are available
                for (int i = 0; i < Deck.m_Deck.size(); i++) {
                    if (Deck.m_Deck[i].m_Count > 0)
                        CardsAvail = true;
                }
                // if no available cards
                //if (!CardsAvail)
                    // Lose Game State

                while (CardsAvail) {
                    // Randomize card spawned
                    int cardindex = rand() % 3;
                    // If card is available
                    if (Deck.m_Deck[cardindex].m_Count > 0) {
                        // Decrease available card count
                        Deck.m_Deck[cardindex].m_Count--;

                        // Spawn Card
                        // Check if GID is Valid
                        if (Deck.m_Deck[cardindex].m_CardGID == paperback::settings::invalid_index_v) return; // Check Archetype* rather than GID, default value for uninitialized variables are prolly 0
                        // Get Unit Info and Spawn unit
                        auto PrefabInfo = m_Coordinator.GetEntityInfo(Deck.m_Deck[cardindex].m_CardGID);
                        auto InstanceGID = PrefabInfo.m_pArchetype->ClonePrefab(PrefabInfo.m_PoolDetails.m_PoolIndex);
                        // Update Card properties
                        auto m_obj = m_Coordinator.GetEntityInfo(InstanceGID);
                        // Update Position
                        transform* Transform = &m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
                        Transform->m_Position = Deck.m_Position[PositionIndex];
                        // Update Card Index (position)
                        card* Card = &m_obj.m_pArchetype->GetComponent<card>(m_obj.m_PoolDetails);
                        Card->m_PositionIndex = PositionIndex;
                        break;
                    }
                }
            });

        // Delete Card
        m_Coordinator.DeleteEntity(Entity);
    }
};