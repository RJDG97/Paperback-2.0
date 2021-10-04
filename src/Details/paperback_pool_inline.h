	#pragma once

namespace paperback::vm
{
	u32 instance::GetPageIndex( const size_t LocalComponentIndex, const u32 Count ) const noexcept
	{
		return ( m_ComponentInfo[LocalComponentIndex]->m_Size * Count ) / settings::virtual_page_size_v;
	}

	u32 instance::GetPageIndex( const component::info& Info, const u32 Count ) const noexcept
	{
		return ( ( Info.m_Size * Count ) - 1 ) / settings::virtual_page_size_v;
	}

	void instance::Clear() noexcept
	{
		while ( m_CurrentEntityCount )
		{
			Delete( m_CurrentEntityCount-1 );
		}
	}

	instance::~instance() noexcept
	{
		Clear();

		for ( auto cPool : m_ComponentPool )
		{
			if ( cPool )
				VirtualFree( cPool, 0, MEM_RELEASE );
		}
	}

	void instance::Init( std::span<const component::info* const> Types, const u32 NumComponents ) noexcept
	{
		m_ComponentInfo		 = Types;
		m_NumberOfComponents = NumComponents;

		// Reserve memory required for MaxEntites
		for ( std::size_t i = 0; i < m_NumberOfComponents; i++ )
		{
			auto nPages = GetPageIndex( *m_ComponentInfo[i], settings::max_entities_v ) + 1;
			m_ComponentPool[i] = reinterpret_cast<std::byte*>( VirtualAlloc(nullptr, nPages * paperback::settings::virtual_page_size_v, MEM_RESERVE, PAGE_NOACCESS) );
			assert( m_ComponentPool[i] );
		}
	}


	u32 instance::Append() noexcept
	{
		assert( m_CurrentEntityCount < settings::max_entities_v );

		// For each valid component
		for (size_t i = 0, end = m_NumberOfComponents; i < end; i++)
		{
			auto CInfo	   = *m_ComponentInfo[i];
			auto iCurPage  =  GetPageIndex( CInfo, m_CurrentEntityCount );
			auto iNextpage =  GetPageIndex( CInfo, m_CurrentEntityCount + 1 );

			// Commit new memory in m_ComponentPool's page if the page is full
			if ( iCurPage != iNextpage )
			{
				auto pEndOfCurrentPool = m_ComponentPool[i] + iNextpage * paperback::settings::virtual_page_size_v;
				auto pNewPool = VirtualAlloc(pEndOfCurrentPool, paperback::settings::virtual_page_size_v, MEM_COMMIT, PAGE_READWRITE);

				assert( pNewPool == pEndOfCurrentPool );
			}

			// Invoke constructor for Component (If Required)
			if ( m_ComponentInfo[i]->m_Constructor )
			{
				m_ComponentInfo[i]->m_Constructor(m_ComponentPool[i] + m_CurrentEntityCount * m_ComponentInfo[i]->m_Size);
			}
		}

		return m_CurrentEntityCount++;
	}

	u32 instance::Delete( const u32 PoolIndex ) noexcept
	{
		assert( PoolIndex < m_CurrentEntityCount && 
				PoolIndex >= 0 );

		// Return back to the current index
		--m_CurrentEntityCount;

		for ( size_t i = 0; i < m_NumberOfComponents; ++i )
		{
			const auto& pInfo = *m_ComponentInfo[i];
			auto		pData =  m_ComponentPool[i];

			// Deleting last Entity
			if ( PoolIndex == m_CurrentEntityCount )
			{
				if ( pInfo.m_Destructor )
					pInfo.m_Destructor( &pData[m_CurrentEntityCount * pInfo.m_Size] );
			}
			// Deleting any other Entity
			else
			{
				if ( pInfo.m_Move )
				{
					pInfo.m_Move( &pData[PoolIndex * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size] );
				}
				else
				{
					if ( pInfo.m_Destructor )
						pInfo.m_Destructor(&pData[PoolIndex * pInfo.m_Size]);
					memcpy( &pData[PoolIndex * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size], pInfo.m_Size );
				}
			}

			// Free the page if empty
			const auto LastPage = GetPageIndex( pInfo, m_CurrentEntityCount + 1 );
			if ( LastPage != GetPageIndex(pInfo, m_CurrentEntityCount) )
			{
				auto pRaw = &pData[ paperback::settings::virtual_page_size_v * LastPage ];
				auto b = VirtualFree( pRaw, paperback::settings::virtual_page_size_v, MEM_DECOMMIT );
				assert( b );
			}
		}

		return ( m_CurrentEntityCount == 0 || PoolIndex >= m_CurrentEntityCount )
				 ? UINT32_MAX
				 : GetComponent<component::entity>( PoolIndex ).m_GlobalIndex;
	}

	void instance::RemoveTransferredEntity( const u32 PoolIndex ) noexcept
	{
		assert( PoolIndex >= 0 );

		// Backtrack to last valid entity
		while ( m_CurrentEntityCount )
        {
			--m_CurrentEntityCount;
            if ( GetComponent<paperback::component::entity>( m_CurrentEntityCount ).IsZombie() == false ) break;
        }

		if ( PoolIndex >= m_CurrentEntityCount )
			return;

		for ( size_t i = 0; i < m_NumberOfComponents; ++i )
		{
			const auto& pInfo = *m_ComponentInfo[i];
			auto		pData =  m_ComponentPool[i];

			// If moving last entity - Ignore
			if ( PoolIndex == m_CurrentEntityCount )
				return;
			// Moving any other entity - Copy memory and shift forward
			else
			{
				if ( pInfo.m_Move )
				{
					pInfo.m_Move( &pData[PoolIndex * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size] );
				}
				else
				{
					memcpy( &pData[PoolIndex * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size], pInfo.m_Size );
				}
			}

			// Free the page if empty
			const auto LastPage = GetPageIndex( pInfo, m_CurrentEntityCount + 1 );
			if ( LastPage != GetPageIndex(pInfo, m_CurrentEntityCount) )
			{
				auto pRaw = &pData[ paperback::settings::virtual_page_size_v * LastPage ];
				auto b = VirtualFree( pRaw, paperback::settings::virtual_page_size_v, MEM_DECOMMIT );
				assert( b );
			}
		}
	}

	u32 instance::TransferExistingComponents( const PoolDetails& Details, vm::instance& FromPool ) noexcept
	{
		const u32 NewPoolIndex = Append();

        u32 iPoolFrom = 0;
        u32 iPoolTo   = 0;

        while( true )
        {
			// Component exists in both - Copy existing component
            if ( FromPool.m_ComponentInfo[ iPoolFrom ]->m_Guid == m_ComponentInfo[ iPoolTo ]->m_Guid )
            {
                auto& Info = *( FromPool.m_ComponentInfo[ iPoolFrom ] );

                if( Info.m_Move )
                {
                    Info.m_Move( &m_ComponentPool[ iPoolTo ][ Info.m_Size * NewPoolIndex ]					    // Destination
							   , &FromPool.m_ComponentPool[ iPoolFrom ][ Info.m_Size * Details.m_PoolIndex ] ); // Source
                }
                else
                {
                    std::memcpy( &m_ComponentPool[ iPoolTo ][ Info.m_Size * NewPoolIndex ]						// Destination
                               , &FromPool.m_ComponentPool[ iPoolFrom ][ Info.m_Size * Details.m_PoolIndex ]	// Source
                               , Info.m_Size );																	// Number of bytes to copy
                }

				// If either pool's components is maxed out
                if ( ++iPoolFrom >= FromPool.m_ComponentInfo.size() || ++iPoolTo >= m_ComponentInfo.size() ) break;
            }
			// Component has been removed in new archetype - Destroy old component
            else if ( FromPool.m_ComponentInfo[ iPoolFrom ]->m_UID < m_ComponentInfo[ iPoolTo ]->m_UID )
            {
                auto& Info = *( FromPool.m_ComponentInfo[ iPoolFrom ] );

                if( Info.m_Destructor )
					Info.m_Destructor( &FromPool.m_ComponentPool[ iPoolFrom ][ Info.m_Size * Details.m_PoolIndex ] );

                if ( ++iPoolFrom >= FromPool.m_ComponentInfo.size() ) break;
            }
			// Component does not exist in old pool - Continue
            else
            {
                if ( ++iPoolTo >= m_ComponentInfo.size() ) break;
            }
        }

        // Remove components from previous archetype post iterating last component from new archetype
        while ( iPoolFrom < FromPool.m_ComponentInfo.size() )
        {
            auto& Info = *( FromPool.m_ComponentInfo[ iPoolFrom ] );

            if ( Info.m_Destructor )
				Info.m_Destructor( &FromPool.m_ComponentPool[ iPoolFrom ][ Info.m_Size * Details.m_PoolIndex ] );
            
            if ( ++iPoolFrom >= FromPool.m_ComponentInfo.size() ) break;
        }

		// Update moved entity to be a zombie
		auto& MovedEntity				   = FromPool.GetComponent<paperback::component::entity>( Details.m_PoolIndex );
		MovedEntity.m_Validation.m_bZombie = true;

        return NewPoolIndex;
	}

	template < typename T_COMPONENT >
	T_COMPONENT& instance::GetComponent( const u32 PoolIndex ) const noexcept
	{
		auto ComponentIndex = GetComponentIndex( component::info_v<T_COMPONENT>.m_UID );

		return *reinterpret_cast< std::decay_t<T_COMPONENT>* >
		(
			&m_ComponentPool[ ComponentIndex ][ PoolIndex * m_ComponentInfo[ComponentIndex]->m_Size] 
		);
	}

	int instance::GetComponentIndex( const u32 UIDComponent ) const noexcept
	{
		// Find index of component within m_ComponentPool
		for ( size_t i = 0, end = m_NumberOfComponents; i < end; ++i )
			if ( m_ComponentInfo[i]->m_UID == UIDComponent ) { return static_cast<int>(i); }

		assert( false );
		return -1;
	}

	int instance::GetComponentIndexFromGUID( const component::type::guid Guid ) const noexcept
	{
		for ( u32 i = 0, max = m_NumberOfComponents; i < max; ++i )
		{
			if ( m_ComponentInfo[i]->m_Guid.m_Value == Guid.m_Value )
				return i;
		}
		return -1; // Replace with an error - No Crash
	}

	int instance::GetComponentIndexFromGUIDInSequence( const component::type::guid Guid, const int Sequence ) const noexcept
	{
		for ( u32 i = Sequence, max = m_NumberOfComponents; i < max; ++i )
		{
			if ( m_ComponentInfo[i]->m_Guid.m_Value == Guid.m_Value )
				return i;
		}
		return -1;
	}

	void instance::SerializePoolComponentsAtEntityIndex( const u32 Index, paperback::JsonFile& Jfile ) noexcept
	{
		//here access to each component in the pool and serializes them
		for ( u32 i = 0, max = m_NumberOfComponents; i < max; ++i )
		{
			rttr::instance Component = GetComponentInstance( m_ComponentInfo[i]->m_Guid, Index );
			Jfile.WriteKey( Component.get_type().get_name().to_string() ).StartObject();
			Jfile.Write( Component );
			Jfile.EndObject();

		}
	}

	rttr::instance instance::GetComponentInstance( const component::type::guid Comp_Guid, const u32 Index ) noexcept
	{

		if ( Comp_Guid.m_Value == component::info_v< component::entity >.m_Guid.m_Value )
			return rttr::instance( GetComponent< component::entity >( Index ));
		else if ( Comp_Guid.m_Value == component::info_v< transform >.m_Guid.m_Value )
			return rttr::instance( GetComponent< transform >( Index ));
		else if ( Comp_Guid.m_Value == component::info_v< rigidbody >.m_Guid.m_Value )
			return rttr::instance( GetComponent< rigidbody >( Index ));
		else if ( Comp_Guid.m_Value == component::info_v< timer >.m_Guid.m_Value )
			return rttr::instance( GetComponent< timer >( Index ));
		else
			return rttr::instance();
	}	

}