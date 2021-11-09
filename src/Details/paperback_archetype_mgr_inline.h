#pragma once

namespace paperback::archetype
{
    //-----------------------------------
    //             Default
    //-----------------------------------
    manager::manager( paperback::coordinator::instance& Coordinator ) :
        m_Coordinator{ Coordinator }
    { }

    void manager::Initialize( void ) noexcept
    {
        for ( u32 i = 0, max = settings::max_entities_v - 2; i < max; ++i )
        {
            m_EntityInfos[ i ].m_PoolDetails.m_PoolIndex = i + 1;
        }

        m_EntityHead = 0;
    }


    //-----------------------------------
    //             Create
    //-----------------------------------

    template < typename T_FUNCTION >
    void manager::CreateEntity( T_FUNCTION&& Function ) noexcept
    {
        using func_traits = xcore::function::traits<T_FUNCTION>;

	    [&] <typename... T_COMPONENTS>( std::tuple<T_COMPONENTS...>* )
	    {
		    auto& Archetype = GetOrCreateArchetype<T_COMPONENTS...>( );
            Archetype.CreateEntity( Function );

	    }( reinterpret_cast<typename func_traits::args_tuple*>(nullptr) );
    }

    void manager::CreatePrefab( void ) noexcept
    {
		auto& Archetype = GetOrCreateArchetype<prefab, transform>( );
        Archetype.CreatePrefab( );
    }

    template < typename... T_COMPONENTS >
    archetype::instance& manager::GetOrCreateArchetype( void ) noexcept
    {
        static constexpr auto ComponentList = paperback::component::sorted_info_array_v< std::tuple<component::entity, T_COMPONENTS...> >;
        return GetOrCreateArchetype( ComponentList, sizeof...( T_COMPONENTS ) + 1 );
    }

    archetype::instance& manager::GetOrCreateArchetype( const tools::bits ArchetypeSignature ) noexcept
    {
        for ( auto& ArchetypeBits : m_ArchetypeBits )
        {
            if ( ArchetypeBits.Match( ArchetypeSignature ) )
            {
                const auto index = static_cast<size_t>( &ArchetypeBits - &m_ArchetypeBits[0] );
                return *m_pArchetypeList[ index ];
            }
        }

        u32 Count = 0;
        std::array<const paperback::component::info*, settings::max_components_per_entity_v> InfoList;

        for ( u32 i = 0, max = settings::max_component_types_v; i < max; ++i )
        {
            if ( Count >= 32 ) break;
            if ( ArchetypeSignature.Has( i ) )
                InfoList[ Count++ ] = m_Coordinator.FindComponentInfoFromUID( i );
        }

        auto& Archetype = CreateAndInitializeArchetype( InfoList, Count, ArchetypeSignature );

        return Archetype;
    }

    template < concepts::Callable T_FUNCTION >
	component::entity manager::AddOrRemoveComponents( const component::entity Entity
								                    , std::span<const component::info* const> Add
								                    , std::span<const component::info* const> Remove
								                    , T_FUNCTION&& Function ) noexcept
	{
		PPB_ASSERT_MSG( Entity.IsZombie(), "Attempting to add component to non-existent entity" );

		auto& EntityInfo        = GetEntityInfo( Entity.m_GlobalIndex );
        auto  OriginalArchetype = EntityInfo.m_pArchetype;
		auto  UpdatedSignature  = EntityInfo.m_pArchetype->GetComponentBits();

        // Validifies Add / Remove
        auto InvalidAddRemove = [&]( const tools::bits& Sig, std::span<const component::info* const> A, std::span<const component::info* const> R ) noexcept -> bool
        {
            for ( auto& ToAdd : A )
                if ( Sig.Has( ToAdd->m_UID ) ) return true;
            for ( auto& ToRemove : R )
                if ( Sig.None( PPB.FindComponentInfo(ToRemove->m_Guid)->m_UID) ) return true;
            return false;
        };
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

        if ( InvalidAddRemove( UpdatedSignature, Add, Remove ) )
            PPB_ERR_PRINT_N_LOG( "Archetype Manager: Invalid Addition / Removal of Components from Entity" );

		// Update Entity's bit signature based on new components
		for ( const auto& ComponentToAdd : Add )
		{
            if ( InvalidComponentModification( ComponentToAdd ) ) continue;
			UpdatedSignature.Set( ComponentToAdd->m_UID );
		}
        for ( const auto& ComponentToRemove : Remove )
		{
			if ( InvalidComponentModification( ComponentToRemove ) ) continue;
			UpdatedSignature.Remove( PPB.FindComponentInfo( ComponentToRemove->m_Guid )->m_UID );
		}

		auto ExistingArchetype = Search( UpdatedSignature );

        // Update All Prefab Instances If Updated Entity Is A Prefab
        AddOrRemoveComponentsFromPrefabInstances( EntityInfo
                                                , Add
                                                , Remove );

		/*
            If Archetype with matching bit signature already exists
            If it does, transfer components over - But don't delete old entity yet
        */
		if ( ExistingArchetype )
		{
            if ( ExistingArchetype != OriginalArchetype )
            {
			    if ( std::is_same_v<T_FUNCTION, empty_lambda> ) return ExistingArchetype->TransferExistingEntity( Entity );
			    else											return ExistingArchetype->TransferExistingEntity( Entity, Function );
            }
		}
		// Create Archetype with matching bit signature
		else
		{
            using CInfoArr = std::array< const paperback::component::info*
                                       , settings::max_components_per_entity_v >;

			int Count{ };
            CInfoArr NewComponentInfoList;
			auto& Archetype = *EntityInfo.m_pArchetype;

			for ( auto& CInfo : std::span{ Archetype.GetComponentInfos().data(), Archetype.GetComponentCount() } )
				NewComponentInfoList[Count++] = CInfo;

			for ( auto& CInfo : Add )
			{
                const auto Index = paperback::component::details::find_component_index_v<CInfoArr>( NewComponentInfoList, CInfo, Count );

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
                const auto Index = paperback::component::details::find_component_index_v<CInfoArr>( NewComponentInfoList, CInfo, Count );

                // Modifying component::entity
                if ( InvalidComponentIndex(Index) ) continue;

				if ( NewComponentInfoList[ Index - 1 ]->m_Guid == CInfo->m_Guid )
				{
					std::memmove( &NewComponentInfoList[ Index - 1 ], &NewComponentInfoList[ Index ], ( Count - Index ) * sizeof( component::info* ) );
					--Count;
				}
			}
            
            // Construct New Archetype
			auto& NewArchetype = CreateAndInitializeArchetype( NewComponentInfoList, Count, UpdatedSignature );

            /*
                Transfer components over to new archetype - But don't delete old entity yet
            */
			if constexpr ( std::is_same_v<T_FUNCTION, paperback::empty_lambda> )   return NewArchetype.TransferExistingEntity( Entity );
			else                                                                   return NewArchetype.TransferExistingEntity( Entity, Function );
		}

        // Do check the return value
        return paperback::component::entity
               {
                   .m_GlobalIndex = paperback::settings::invalid_index_v
               };
	}


    //-----------------------------------
    //             Getters
    //-----------------------------------

    entity::info& manager::GetEntityInfo( const component::entity Entity ) const noexcept
    {
        return m_EntityInfos[ Entity.m_GlobalIndex ];
    }

    entity::info& manager::GetEntityInfo( const u32 GlobalIndex ) const noexcept
    {
        return m_EntityInfos[ GlobalIndex ];
    }

    archetype::instance* manager::FindArchetype( const u64& ArchetypeGuid ) const noexcept
    {
        auto a = m_pArchetypeMap.find(ArchetypeGuid);
        if (a == m_pArchetypeMap.end()) return nullptr;
        return a->second;
    }

    archetype::instance& manager::GetArchetype( const u64& ArchetypeGuid ) const noexcept
    {
        auto a = FindArchetype(ArchetypeGuid);
        PPB_ASSERT_MSG(a == nullptr, "Archetype does not exist / Invalid Archetype Guid Value");
        return *a;
    }

    std::vector<paperback::archetype::instance*> manager::GetArchetypeList( void ) noexcept
    {
        std::vector<paperback::archetype::instance*> List;
        for ( auto& Archetype : m_pArchetypeList )
            List.push_back( Archetype.get() );
        return List;
    }

    manager::EntityInfoList& manager::GetEntityInfoList() noexcept
    {
        return m_EntityInfos;
    }

    u32 manager::GetEntityHead() noexcept
    {
        return m_EntityHead;
    }

    void manager::SetEntityHead( u32 NewEntityHead ) noexcept
    {
        m_EntityHead = NewEntityHead;
    }


    //-----------------------------------
    //             Query
    //-----------------------------------
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

                if ( m_pArchetypeList[index]->GetCurrentEntityCount() > 0 )
                    ValidArchetypes.push_back( m_pArchetypeList[index].get() );
            }
        }

        return ValidArchetypes;
    }

    
    //-----------------------------------
    //     Update Prefab Instances
    //-----------------------------------
    
    template < typename T_COMPONENT >
    void manager::UpdatePrefabInstancesOnPrefabComponentUpdate( const entity::info& PrefabInfo
                                                              , const T_COMPONENT&  UpdatedComponent ) noexcept
    {
        auto& Prefab                  = PrefabInfo.m_pArchetype->GetComponent<prefab>( PrefabInfo.m_PoolDetails );
        auto& PrefabInstanceArchetype = *( m_Coordinator.GetEntityInfo( *(Prefab.m_ReferencePrefabGIDs.begin()) ).m_pArchetype );

		PPB_ASSERT_MSG( PrefabInstanceArchetype.GetCurrentEntityCount() != Prefab.m_ReferencePrefabGIDs.size(),
                        "Different Prefab Instance Counts in PrefabInstanceArchetype & ReferencePrefabGIDs" );

        for ( size_t i = 0, max = Prefab.m_ReferencePrefabGIDs.size(); i < max; ++i )
        {
            auto& Reference_Prefab = PrefabInstanceArchetype.GetComponent<reference_prefab>( vm::PoolDetails{ .m_Key = 0, .m_PoolIndex = i } );
            auto& Info             = component::info_v<T_COMPONENT>;

            if ( !(Reference_Prefab.HasModified( Info.m_Guid.m_Value )) )
            {
                // Update Prefab Instance's Component
                PrefabInstanceArchetype.GetComponent<T_COMPONENT>( vm::PoolDetails{ .m_Key = 0, .m_PoolIndex = i } ) = UpdatedComponent;
            }
        }
    }

    //-----------------------------------
    //              Clear
    //-----------------------------------

    void manager::ResetAllArchetypes( void ) noexcept
    {
        for ( auto& Archetype : m_pArchetypeList )
        {
            Archetype->Clear();
        }

        //m_pArchetypeList.clear();
    }

    void manager::Terminate( void ) noexcept
    {
        // Delete All Entities
    }


    //-----------------------------------
    //             Helper
    //-----------------------------------

    PPB_INLINE
    paperback::component::entity& manager::RegisterEntity( const PoolDetails Details
                                     , archetype::instance& Archetype ) noexcept
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

        auto& Entity = Archetype.GetComponent<paperback::component::entity>( EntityInfo.m_PoolDetails );

        Entity.m_GlobalIndex          = EntityGlobalIndex;
        Entity.m_Validation.m_Next    = 0;
        Entity.m_Validation.m_bZombie = false;

        return Entity;
    }

    PPB_INLINE
    void manager::RemoveEntity( const u32 SwappedGlobalIndex
                                   , const u32 DeletedEntityIndex ) noexcept
    {
        auto& Info = GetEntityInfo( DeletedEntityIndex );

        // If Deleted Entity is not last entity in Pool
        if ( SwappedGlobalIndex != paperback::settings::invalid_index_v )           
        {
            m_EntityInfos[SwappedGlobalIndex].m_PoolDetails = Info.m_PoolDetails;
        }

        Info.m_PoolDetails.m_PoolIndex = m_EntityHead;
        m_EntityHead                   = DeletedEntityIndex;
    }


    //-----------------------------------
    //             Private
    //-----------------------------------

    u32 manager::AppendEntity() noexcept
    {
        PPB_ASSERT_MSG( m_EntityHead == settings::invalid_index_v, "Invalid Global Index Generated" );

        u32 GlobalIndex         = m_EntityHead;
        auto& CurrentEntityInfo = m_EntityInfos[ m_EntityHead ];
        m_EntityHead            = CurrentEntityInfo.m_PoolDetails.m_PoolIndex;

        return GlobalIndex;
    }

    archetype::instance& manager::GetOrCreateArchetype( std::span<const component::info* const> Types
                                                      , const u32 Count ) noexcept
    {
        tools::bits ArchetypeSignature;

        for ( const auto& CInfo : Types )
            ArchetypeSignature.Set( CInfo->m_UID );

        for ( auto& ArchetypeBits : m_ArchetypeBits )
        {
            if ( ArchetypeBits.Compare( ArchetypeSignature ) )
            {
                const auto index = static_cast<size_t>( &ArchetypeBits - &m_ArchetypeBits[0] );
                return *( m_pArchetypeList[ index ] );
            }
        }

        return CreateAndInitializeArchetype( Types, Count, ArchetypeSignature );
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

    archetype::instance& manager::CreateAndInitializeArchetype( std::span<const component::info* const> Types
                                                              , const u32 Count
                                                              , const tools::bits& Signature ) noexcept
    {
        m_pArchetypeList.push_back( std::make_unique<archetype::instance>( m_Coordinator, Signature ) );
		m_ArchetypeBits.push_back( Signature );

        auto p = m_pArchetypeList.back().get();

        p->Init( Types, Count );
        if (m_pArchetypeMap.find(p->GetArchetypeGuid().m_Value) != m_pArchetypeMap.end())
            m_pArchetypeMap[p->GetArchetypeGuid().m_Value] = p;
        else
            m_pArchetypeMap.emplace( std::make_pair( p->GetArchetypeGuid().m_Value, p ) );

        return *( m_pArchetypeList.back() );
    }

    void manager::AddOrRemoveComponentsFromPrefabInstances( const entity::info& Info
                                                          , std::span<const component::info* const> Add
								                          , std::span<const component::info* const> Remove ) noexcept
    {
        // Check if Archetype is a Prefab
        auto Prefab = Info.m_pArchetype->FindComponent<prefab>( vm::PoolDetails{ .m_Key = 0, .m_PoolIndex = 0 } );

        if ( Prefab && Prefab->m_ReferencePrefabGIDs.size() )
        {
            auto& PrefabInstanceArchetype = *( m_Coordinator.GetEntityInfo( *(Prefab->m_ReferencePrefabGIDs.begin()) ).m_pArchetype );

            PPB_ASSERT_MSG( PrefabInstanceArchetype.GetCurrentEntityCount() != Prefab->m_ReferencePrefabGIDs.size(),
                            "Different Prefab Instance Counts in PrefabInstanceArchetype & ReferencePrefabGIDs" );

            for ( size_t i = 0, max = Prefab->m_ReferencePrefabGIDs.size(); i < max; ++i )
            {
                auto& Reference_Prefab = PrefabInstanceArchetype.GetComponent<reference_prefab>( vm::PoolDetails{ .m_Key = 0, .m_PoolIndex = 0 } );

                // Unassign Modified Guid Values On Component Removal
                for ( const auto& ComponentToRemove : Remove )
		        {
                    if ( Reference_Prefab.m_ModifiedComponents.size() )
                    {
                        Reference_Prefab.RemoveModifiedComponentGuid( ComponentToRemove->m_Guid.m_Value );
                    }
                }

                AddOrRemoveComponents( PrefabInstanceArchetype.GetComponent<paperback::component::entity>( vm::PoolDetails{ .m_Key = 0, .m_PoolIndex = 0 } )
                                     , Add
                                     , Remove
                                     );
            }
        }
    }

	//void manager::UpdateReferencedPrefabInstanceOnAddRemove( const entity::info& Info
 //                                                          , const u64 ComponentGuid ) noexcept
 //   {
 //       auto Reference_Prefab = Info.m_pArchetype->FindComponent<reference_prefab>( Info.m_PoolDetails );

 //       if ( Reference_Prefab && Reference_Prefab->m_ModifiedComponents.size() )
 //       {
 //           Reference_Prefab->RemoveModifiedComponentGuid( ComponentGuid );
 //       }
 //   }
}