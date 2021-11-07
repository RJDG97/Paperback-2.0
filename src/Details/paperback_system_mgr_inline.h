#pragma once

namespace paperback::system
{
	manager::manager( tools::clock& Clock ) :
		m_SystemClock{ Clock }
	{ }

	manager::~manager()
	{
		m_SystemMap.clear();

		while( m_Systems.size() )
        {
            auto p = m_Systems.back().second.release();
			m_Systems.back().first->m_Destructor( *p );
            delete reinterpret_cast<void*>( p );
			m_Systems.pop_back();
        }
	}
	
	template < typename... T_SYSTEMS >
	constexpr void manager::RegisterSystems( coordinator::instance& Coordinator ) noexcept
	{
		( (RegisterSystem<T_SYSTEMS>( Coordinator )), ... );
	}

	template < typename T_SYSTEM >
	constexpr T_SYSTEM& manager::RegisterSystem( coordinator::instance& Coordinator ) noexcept
	{
		using system_t  = system::details::completed<T_SYSTEM>;
		using typedef_t = std::decay_t<decltype( T_SYSTEM::typedef_v )>;

		// Register System
		m_Systems.push_back({ &system::info_v<T_SYSTEM>, std::make_unique< system::details::completed<T_SYSTEM> >(Coordinator) });

		// Initialize Pointers
		auto* pInfo   = m_Systems.back().first;
		auto* pSystem = m_Systems.back().second.get();

		// Run OnCreated()
		if constexpr ( &T_SYSTEM::OnSystemCreated != &system_interface::OnSystemCreated )
			static_cast<T_SYSTEM*>( pSystem )->OnSystemCreated();

		// Save Info
		m_SystemMap.emplace( std::make_pair( system::info_v<T_SYSTEM>.m_Guid, pSystem ) );

		// Set System Query In Advance
		if constexpr ( T_SYSTEM::typedef_v.id_v != paperback::system::type::id::SYSTEM_EVENT )
			InitializeSystemQuery<T_SYSTEM>();

		// If System is an Event System
		if constexpr ( T_SYSTEM::typedef_v.id_v == paperback::system::type::id::SYSTEM_EVENT )
		{
			// Grab Event Instance from T_SYSTEM's tuple
			std::get< typedef_t::EventType >( reinterpret_cast< system::details::completed<typedef_t::SystemType>* >( FindSystem< typedef_t::SystemType >() )->m_Events )
				.RegisterEvent< &T_SYSTEM::OnEvent >( reinterpret_cast<system_t*>( pSystem ) );
		}
		// If System is an Update System
		else
		{
			// Initialize and Save System Functionality as Events
			InitializeSystemUpdateEvents( static_cast<T_SYSTEM*>( pSystem ) );
		}

		return *( static_cast<T_SYSTEM*>( pSystem ) );
	}

	template < typename T_SYSTEM >
	T_SYSTEM* manager::FindSystem( void ) noexcept
	{
		return m_SystemMap.find( system::info_v<T_SYSTEM>.m_Guid ) != m_SystemMap.end()
			   ? static_cast< T_SYSTEM* >( m_SystemMap.find( system::info_v<T_SYSTEM>.m_Guid )->second )
			   : nullptr;
	}
	
	void manager::Run( void ) noexcept
	{
		// Track Frame Time
		m_SystemClock.Tick();

		m_Events.m_OnFrameStart.BroadcastEvent();
		m_Events.m_OnPreUpdate.BroadcastEvent();
		m_Events.m_OnUpdate.BroadcastEvent();
		m_Events.m_OnPostUpdate.BroadcastEvent();
		m_Events.m_OnFrameEnd.BroadcastEvent();
	}

	void manager::Terminate( void ) noexcept
	{
		m_Events.m_OnSystemTerminated.BroadcastEvent();
	}


	template < paperback::concepts::System T_SYSTEM >
	void manager::InitializeSystemQuery( void ) noexcept
	{
		tools::query SystemQuery{};

		SystemQuery.AddQueryFromTuple( xcore::types::null_tuple_v< T_SYSTEM::query > );
        if constexpr ( xcore::function::is_callable_v<T_SYSTEM> )
        {
			SystemQuery.AddQueryFromFunction<T_SYSTEM>( );
        }

        system::info_v<T_SYSTEM>.m_Query = SystemQuery;
	}

	template < paperback::concepts::System T_SYSTEM >
	void manager::InitializeSystemUpdateEvents( T_SYSTEM* System ) noexcept
	{
		using system_t = system::details::completed<T_SYSTEM>;

		if constexpr ( &T_SYSTEM::OnFrameStart != &system_interface::OnFrameStart )
			m_Events.m_OnFrameStart.RegisterEvent< &system_t::System_OnFrameStart >( static_cast<system_t*>( System ) );

		if constexpr ( &T_SYSTEM::PreUpdate != &system_interface::PreUpdate)
			m_Events.m_OnPreUpdate.RegisterEvent< &system_t::PreUpdate >( static_cast<system_t*>( System ) );

		m_Events.m_OnUpdate.RegisterEvent< &system_t::System_OnUpdate >( static_cast<system_t*>( System ) );

		if constexpr ( &T_SYSTEM::PostUpdate != &system_interface::PostUpdate )
			m_Events.m_OnPostUpdate.RegisterEvent< &system_t::PostUpdate >( static_cast<system_t*>( System ) );

		if constexpr ( &T_SYSTEM::OnFrameEnd != &system_interface::OnFrameEnd )
			m_Events.m_OnFrameEnd.RegisterEvent< &system_t::System_OnFrameEnd >( static_cast<system_t*>( System ) );

		if constexpr ( &T_SYSTEM::OnSystemTerminated != &system_interface::OnSystemTerminated )
			m_Events.m_OnSystemTerminated.RegisterEvent< &T_SYSTEM::OnSystemTerminated >( static_cast<system_t*>( System ) );
	}
}