#pragma once

namespace paperback::entity
{
    manager::manager( paperback::coordinator::instance& Coordinator ) :
        m_Coordinator{ Coordinator }
    { }

	void manager::RegisterEntity( const PoolDetails Details, archetype::instance& Archetype ) noexcept
    {
        u32   EntityGlobalIndex = AppendEntity();
        auto& EntityInfo        = GetEntityInfo( EntityGlobalIndex );

        EntityInfo = entity::info
                     {
                         .m_pArchetype  = &Archetype
                     ,   .m_PoolDetails = Details
                     ,   .m_Validation  = component::entity::Validation
                         {
                             .m_Next    = 0
                         ,   .m_bZombie = false
                         }
                     };

        auto& Entity = EntityInfo.m_pArchetype->GetComponent<component::entity>( EntityInfo.m_PoolDetails );

        Entity.m_GlobalIndex          = EntityGlobalIndex;
        Entity.m_Validation.m_Next    = 0;
        Entity.m_Validation.m_bZombie = false;
    }

    template < typename T_FUNCTION >
    void manager::CreateEntity( T_FUNCTION&& Function, coordinator::instance& Coordinator ) noexcept
    {
        using func_traits = xcore::function::traits<T_FUNCTION>;

	    [&] <typename... T_COMPONENTS>( std::tuple<T_COMPONENTS...>* )
	    {
		    auto&      Archetype = GetOrCreateArchetype<T_COMPONENTS...>( Coordinator );
            const auto Details   = Archetype.CreateEntity( Function );

		    RegisterEntity( Details, Archetype );
	    }( reinterpret_cast<typename func_traits::args_tuple*>(nullptr) );
    }

    void manager::RemoveEntity( const u32 SwappedGlobalIndex, const component::entity DeletedEntity ) noexcept
    {
        if ( SwappedGlobalIndex != paperback::settings::invalid_index_v )
        {
            auto& Info = GetEntityInfo( DeletedEntity );
            m_EntityInfos[SwappedGlobalIndex].m_PoolDetails = Info.m_PoolDetails;
        }
        m_AvailableIndexes.push( DeletedEntity.m_GlobalIndex );
    }

    template < typename... T_COMPONENTS >
    archetype::instance& manager::GetOrCreateArchetype( coordinator::instance& Coordinator ) noexcept
    {
        static constexpr auto ComponentList = paperback::component::sorted_info_array_v< std::tuple<component::entity, T_COMPONENTS...> >;
        return GetOrCreateArchetype( ComponentList, Coordinator );
    }

    archetype::instance* manager::FindArchetype(const tools::bits& Signature) noexcept
    {
        for (auto& ArchetypeBits : m_ArchetypeBits)
        {
            if (ArchetypeBits.Match(Signature))
            {
                const auto index = static_cast<size_t>(&ArchetypeBits - &m_ArchetypeBits[0]);
                return m_pArchetypeList[index].get();
            }
        }
        return nullptr;
    }

    void manager::ResetAllArchetypes(void) noexcept
    {
        for (auto& Archetype : m_pArchetypeList)
            Archetype->Clear();
    }


    // PRIVATE FN
    archetype::instance& manager::CreateArchetype(const tools::bits& Signature) noexcept
    {
        m_pArchetypeList.push_back( std::make_unique<archetype::instance>( m_Coordinator, Signature ) );
		m_ArchetypeBits.push_back( Signature );

        return *( m_pArchetypeList.back() );
    }

    entity::info& manager::GetEntityInfo( const component::entity Entity ) const noexcept
    {
        return m_EntityInfos[ Entity.m_GlobalIndex ];
    }

    entity::info& manager::GetEntityInfo( const u32 GlobalIndex ) const noexcept
    {
        return m_EntityInfos[ GlobalIndex ];
    }

    archetype::instance& manager::GetArchetypeFromEntity( const u32 EntityID ) const noexcept
    {
        assert( EntityID < m_EntityIDTracker );

        return *( m_EntityInfos[EntityID].m_pArchetype );
    }

    void manager::FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept
    {
        if ( Archetype->m_ProcessReference == 0 && 
             !Archetype->m_DeleteList.empty() )
        {
            for ( auto& Entity : Archetype->m_DeleteList )
            {
                assert( Entity.IsZombie() == true );

                auto& Info = GetEntityInfo( Entity );

                RemoveEntity( Archetype->DeleteEntity( Info.m_PoolDetails ), Entity );
            }

            Archetype->m_DeleteList.clear();
        }
    }

    template < typename... T_COMPONENTS >
    std::vector<archetype::instance*> manager::Search() const noexcept
    {
        static constexpr auto ComponentList = std::array{ &component::info_v<T_COMPONENTS>... };
        return Search( ComponentList );
    }

    archetype::instance* manager::Search( const tools::bits& Bits ) const noexcept
    {
        for ( const auto& ArchetypeBits : m_ArchetypeBits )
        {
            if ( ArchetypeBits.Match( Bits ) )
            {
                const auto index = static_cast<size_t>( &ArchetypeBits - &m_ArchetypeBits[0] );
                return m_pArchetypeList[index].get();
            }
        }
        return nullptr;
    }

    std::vector<archetype::instance*> manager::Search( const tools::query& Query ) const noexcept
    {
        std::vector<archetype::instance*> ValidArchetypes;

        // Search for all Archetypes with valid Bit Signatures
        for ( const auto& ArchetypeBits : m_ArchetypeBits )
        {
            if ( Query.Compare( ArchetypeBits ) )
            {
                const auto index = static_cast<size_t>( &ArchetypeBits - &m_ArchetypeBits[0] );

                if ( m_pArchetypeList[index]->m_EntityCount > 0 )
                    ValidArchetypes.push_back( m_pArchetypeList[index].get() );
            }
        }

        return ValidArchetypes;
    }


    // PRIVATE
    u32 manager::AppendEntity() noexcept
    {
        if (!m_AvailableIndexes.empty())
        {
            u32 GlobalIndex = m_AvailableIndexes.top();
            m_AvailableIndexes.pop();
            return GlobalIndex;
        }
        else
        {
            return m_EntityIDTracker++;
        }
    }

    archetype::instance& manager::GetOrCreateArchetype( std::span<const component::info* const> Types, coordinator::instance& Coordinator ) noexcept
    {
        tools::bits Query;

        for ( const auto& CInfo : Types )
            Query.Set( CInfo->m_UID );

        for ( auto& Archetype : m_ArchetypeBits )
        {
            if ( Archetype.Compare( Query ) )
            {
                const auto index = static_cast<size_t>( &Archetype - &m_ArchetypeBits[0] );
                return *( m_pArchetypeList[ index ] );
            }
        }

        m_pArchetypeList.push_back( std::make_unique<archetype::instance>(Coordinator, Query) );
        m_ArchetypeBits.push_back( Query );

        m_pArchetypeList.back()->Init( Types, Types.size() );

        return *( m_pArchetypeList.back() );
    }

    template < typename... T_COMPONENTS >
	std::vector<archetype::instance*> manager::Search( std::span<const component::info* const> Types ) const noexcept
	{
	    tools::bits Query;
	    std::vector<archetype::instance*> ValidArchetypes;

	    for ( const auto& cInfo : Types )
		    Query.Set( cInfo->m_UID );

	    for ( auto& Archetype : m_ArchetypeBits )
	    {
		    if ( Archetype.Compare( Query ) )
		    {
			    const auto Index = static_cast<size_t>( &Archetype - &m_ArchetypeBits[0] );
			    ValidArchetypes.push_back( m_pArchetypeList[Index].get() );
		    }
	    }

	    return ValidArchetypes;
	}

    // Coordinator -> Entity Mgr -> New / Existing Archetype -> Pool
    template < concepts::Callable T_FUNCTION >
	component::entity manager::AddOrRemoveComponents( const component::entity Entity
								                     , std::span<const component::info* const> Add
								                     , std::span<const component::info* const> Remove
								                     , T_FUNCTION&& Function ) noexcept
	{
		PPB_ASSERT_MSG( Entity.IsZombie(), "Attempting to add component to non-existent entity" );

		auto& EntityInfo        = GetEntityInfo( Entity.m_GlobalIndex );
        auto  OriginalArchetype = EntityInfo.m_pArchetype;
		auto  UpdatedSignature  = EntityInfo.m_pArchetype->m_ComponentBits;

        auto InvalidComponentModification = [&]( const component::info* ComponentInfo ) -> bool
        {
            if ( ComponentInfo->m_UID == 0 )
            {
                WARN_PRINT( "Attempting to add/remove component::entity" );
                WARN_LOG  ( "Attempting to add/remove component::entity" );
                return true;
            }
            return false;
        };
        auto InvalidComponentIndex = [&]( size_t ComponentIndex ) -> bool
        {
            if ( ComponentIndex == 0 )
            {
                WARN_PRINT( "Attempting to add/remove component::entity - No Matching Archetype Case" );
                WARN_LOG  ( "Attempting to add/remove component::entity - No Matching Archetype Case" );
                return true;
            }
            return false;
        };

		// Update Entity's bit signature based on new components
		for ( const auto& ComponentToAdd : Add )
		{
            if ( InvalidComponentModification( ComponentToAdd ) ) continue;
			UpdatedSignature.Set( ComponentToAdd->m_UID );
		}
        for ( const auto& ComponentToRemove : Remove )
		{
			if ( InvalidComponentModification( ComponentToRemove ) ) continue;
			UpdatedSignature.Remove( ComponentToRemove->m_UID );
		}

		auto ExistingArchetype = Search( UpdatedSignature );

		/*
            If Archetype with matching bit signature already exists
            If it does, transfer components over - But don't delete old entity yet
        */
		if ( ExistingArchetype )
		{
			if ( std::is_same_v<T_FUNCTION, empty_lambda> ) return ExistingArchetype->TransferExistingEntity( Entity );
			else											return ExistingArchetype->TransferExistingEntity( Entity, Function );
		}
		// Create Archetype with matching bit signature
		else
		{
			int Count{ };
			auto& Archetype = *EntityInfo.m_pArchetype;
			std::array<const paperback::component::info*, settings::max_components_per_entity_v > NewComponentInfoList;

			for ( auto& CInfo : std::span{ Archetype.m_ComponentInfos.data(), Archetype.m_NumberOfComponents } )
				NewComponentInfoList[Count++] = CInfo;

			for ( auto& CInfo : Add )
			{
				//const auto Index = component::details::find_component_index_v( NewComponentInfoList, CInfo, Count );
                const auto Index = static_cast<std::size_t>(std::upper_bound(NewComponentInfoList.begin(), NewComponentInfoList.begin() + Count, CInfo, [](const paperback::component::info* pA, const paperback::component::info* pB)
                {
                    return pA->m_Guid < pB->m_Guid;
                }) - NewComponentInfoList.begin());

                // Modifying component::entity
				if ( InvalidComponentIndex( Index ) ) continue;
				// Avoid dupe components
				if ( NewComponentInfoList[ Index - 1 ]->m_Guid == CInfo->m_Guid ) continue;
				// Shift all component::info*'s backwards by 1 component::info* slot to make space
				if ( Index != Count ) std::memmove( &NewComponentInfoList[ Index + 1 ],
													&NewComponentInfoList[ Index ],
													( Count - Index ) * sizeof( component::info* ) );

				NewComponentInfoList[ Index ] = CInfo;
				++Count;
			}
			for ( auto& CInfo : Remove )
			{
				//const auto Index = component::details::find_component_index_v( NewComponentInfoList, CInfo, Count );
                const auto Index = static_cast<std::size_t>(std::upper_bound(NewComponentInfoList.begin(), NewComponentInfoList.begin() + Count, CInfo, [](const paperback::component::info* pA, const paperback::component::info* pB)
                {
                    return pA->m_Guid < pB->m_Guid;
                }) - NewComponentInfoList.begin());

                // Modifying component::entity
                if ( InvalidComponentIndex(Index) ) continue;

				if ( NewComponentInfoList[ Index - 1 ]->m_Guid == CInfo->m_Guid )
				{
					std::memmove( &NewComponentInfoList[ Index - 1 ], &NewComponentInfoList[ Index ], ( Count - Index ) * sizeof( component::info* ) );
					--Count;
				}
			}
            
            // Initialize Newly Created Archetype
			auto& NewArchetype = CreateArchetype( m_Coordinator, UpdatedSignature );
			NewArchetype.Init( NewComponentInfoList, Count );

            /*
                Transfer components over to new archetype - But don't delete old entity yet
            */
			if constexpr ( std::is_same_v<T_FUNCTION, paperback::empty_lambda> )   return NewArchetype.TransferExistingEntity( Entity );
			else                                                                   return NewArchetype.TransferExistingEntity( Entity, Function );
		}
	}

    void manager::Terminate( void ) noexcept
    {
        // Delete All Entities
    }
}