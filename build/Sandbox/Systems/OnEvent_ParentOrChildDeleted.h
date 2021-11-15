#pragma once

struct onevent_ParentDeleted_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "OnEvent_ParentDeleted"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass< paperback::vm::instance::OnEvent_ParentDeleted >( this );
    }

    PPB_FORCEINLINE
    void operator()( paperback::component::entity& Entity ) noexcept
    {

    }

    void OnEvent( const parent& Parent ) noexcept
    {
		// Deletes children before parent, on deletion of parent
		auto ChildrenList = Parent.m_ChildrenGlobalIndexes;
		
		for ( const auto ChildGID : ChildrenList )
		{
			auto& ChildInfo = GetEntityInfo( ChildGID );
			if ( ChildInfo.m_pArchetype )
				ChildInfo.m_pArchetype->DestroyEntity( ChildInfo.m_pArchetype->GetComponent<paperback::component::entity>( ChildInfo.m_PoolDetails ) );
		}
    }
};


struct onevent_ChildDeleted_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "OnEvent_ChildDeleted"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass< paperback::vm::instance::OnEvent_ChildDeleted >( this );
    }

    PPB_FORCEINLINE
    void operator()( paperback::component::entity& Entity ) noexcept
    {

    }

    void OnEvent( const child& Child, const paperback::u32& GlobalIndex ) noexcept
    {
		// Get parent info
		const auto& ParentInfo = GetEntityInfo( Child.m_ParentGlobalIndex );

		auto& Parent = ParentInfo.m_pArchetype->GetComponent<parent>( ParentInfo.m_PoolDetails );
		Parent.RemoveChild( GlobalIndex );
    }
};