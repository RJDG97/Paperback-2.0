#pragma once

namespace paperback::entity
{
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
        static constexpr auto ComponentList = std::array{ &component::info_v<component::entity>, &component::info_v<T_COMPONENTS>... };
        return GetOrCreateArchetype( ComponentList, Coordinator );
    }

    template < typename... T_COMPONENTS > // PRIVATE FN
    archetype::instance& manager::CreateArchetype( coordinator::instance& Coordinator, const tools::bits& Signature ) noexcept
    {
        m_pArchetypeList.push_back( std::make_unique<archetype::instance>( *this ) );
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

        m_pArchetypeList.back()->Init( Types );

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

    void manager::Terminate( void ) noexcept
    {
        // Delete All Entities
    }
}