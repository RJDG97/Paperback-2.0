#pragma once

namespace paperback::system
{
	//-----------------------------------
	//    System Info Initialization
	//-----------------------------------
	namespace details
    {
        template< typename T_SYSTEM >
        consteval system::type::info CreateInfo( void ) noexcept
        {
            return system::type::info
            {
                .m_Guid          = T_SYSTEM::typedef_v.m_Guid.isValid()
                                   ? T_SYSTEM::typedef_v.m_Guid
                                   : type::guid{ __FUNCSIG__ }
            ,   .m_TypeID        = T_SYSTEM::typedef_v.id_v
			,	.m_Destructor    = []( system::instance& pSystem ) noexcept
								   {
								       std::destroy_at( &static_cast< system::details::completed<T_SYSTEM>& >( pSystem ) ) ;
								   }
			,	.m_pName		   = T_SYSTEM::typedef_v.m_pName
            };
        }
	}


	//-----------------------------------
	//        System Instance
	//-----------------------------------
	instance::instance( coordinator::instance& Coordinator ) noexcept : 
		m_Coordinator{ Coordinator }
	{ }
	

	//-----------------------------------
	//         System Details
	//-----------------------------------
	template < typename USER_SYSTEM >
	details::completed<USER_SYSTEM>::completed( coordinator::instance& Coordinator ) noexcept :
		USER_SYSTEM{ Coordinator }
	{ }

	template < typename USER_SYSTEM >
	void details::completed<USER_SYSTEM>::Run( void ) noexcept
	{
		if constexpr ( &USER_SYSTEM::Update != &system_interface::Update )
			USER_SYSTEM::Update( );
		else
		{
            USER_SYSTEM::m_Coordinator.ForEach( USER_SYSTEM::m_Coordinator.Search( system::info_v<USER_SYSTEM>.m_Query )
											  , *this );
		}
	}


	//-----------------------------------
	//   System Instance Functionality
	//-----------------------------------
	template < typename... T_COMPONENTS >
	archetype::instance& instance::GetOrCreateArchetype( void ) noexcept
	{
		return m_Coordinator.GetOrCreateArchetype<T_COMPONENTS...>();
	}

	template< typename T_FUNCTION >
	void instance::CreateEntity( T_FUNCTION&& Function ) noexcept
	{
		m_Coordinator.CreateEntity( Function );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntities( T_FUNCTION&& Function
					   , const u32 Count ) noexcept
	{
		m_Coordinator.CreateEntities( Function, Count );
	}

	template < typename       T_EVENT
			 , typename       T_SYSTEM
			 , typename...    T_ARGS >
	requires ( std::derived_from< T_SYSTEM, paperback::system::instance >
          && ( !std::is_same_v< typename T_SYSTEM::events, paperback::system::system_interface::events > ))
	void instance::BroadcastEvent( T_SYSTEM* System, T_ARGS&&... Args ) noexcept
	{
		std::get<T_EVENT>( reinterpret_cast< system::details::completed<T_SYSTEM>* >( System )->m_Events )
			.BroadcastEvent( std::forward<T_ARGS&&>( Args ) ... );
	}

	paperback::entity::info& instance::GetEntityInfo( const u32 GlobalIndex ) const noexcept
	{
		return m_Coordinator.GetEntityInfo( GlobalIndex );
	}

	void instance::DeleteEntity( component::entity& Entity ) noexcept
	{
		m_Coordinator.DeleteEntity( Entity );
	}

	template < typename... T_COMPONENTS >
    std::vector<archetype::instance*> instance::Search() const noexcept
	{
		return m_Coordinator.Search<T_COMPONENTS...>();
	}

    std::vector<archetype::instance*> instance::Search( const tools::query& Query ) const noexcept
	{
		return m_Coordinator.Search( Query );
	}

	template < concepts::Callable_Void T_FUNCTION>
    void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList
						  , T_FUNCTION&& Function ) noexcept
	{
		m_Coordinator.ForEach<T_FUNCTION>( ArchetypeList, std::forward<T_FUNCTION&&>( Function ) );
	}

    template < concepts::Callable_Bool T_FUNCTION>
    void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList
						  , T_FUNCTION&& Function ) noexcept
	{
		m_Coordinator.ForEach<T_FUNCTION>( ArchetypeList, std::forward<T_FUNCTION&&>( Function ) );
	}

	template< typename T_SYSTEM >
	T_SYSTEM* instance::FindSystem( void ) noexcept
	{
		return m_Coordinator.FindSystem<T_SYSTEM>();
	}

	template< typename T_SYSTEM >
	T_SYSTEM& instance::GetSystem( void ) noexcept
	{
		return m_Coordinator.GetSystem<T_SYSTEM>();
	}

	PPB_FORCEINLINE
	float instance::DeltaTime() const noexcept
	{
		return m_Coordinator.DeltaTime();
	}
}