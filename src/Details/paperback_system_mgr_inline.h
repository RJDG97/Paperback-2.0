#pragma once

namespace paperback::system
{
	manager::manager( tools::clock& Clock ) :
		m_SystemClock{ Clock }
	{ }
	
	template < typename... T_SYSTEMS >
	constexpr void manager::RegisterSystems( coordinator::instance& Coordinator ) noexcept
	{
		( (RegisterSystem<T_SYSTEMS>( Coordinator )), ... );
	}

	template < typename T_SYSTEM >
	constexpr T_SYSTEM& manager::RegisterSystem( coordinator::instance& Coordinator ) noexcept
	{
		// Register System
		m_Systems.push_back({ &system::info_v<T_SYSTEM>, std::make_unique< system::details::completed<T_SYSTEM> >(Coordinator) });

		// Initialize Pointers
		auto* pInfo   = m_Systems.back().first;
		auto* pSystem = m_Systems.back().second.get();

		// Run OnCreated()
		pInfo->m_RunSystem( *pSystem, system::type::call::CREATED );

		// Save Info
		m_SystemMap.emplace( std::make_pair( system::info_v<T_SYSTEM>.m_Guid, pSystem ) );

		return *( static_cast<T_SYSTEM*>( pSystem ) );
	}

	template < typename T_SYSTEM >
	T_SYSTEM* manager::FindSystem( void ) noexcept
	{
		return m_SystemMap.find( system::info_v<T_SYSTEM>.m_Guid ) != m_SystemMap.end()
			   ? static_cast< T_SYSTEM* >( m_SystemMap.find( system::info_v<T_SYSTEM>.m_Guid )->second )
			   : nullptr;
	}
	// jy
	void manager::Run( void ) noexcept
	{
		// Track Frame Time
		m_SystemClock.Tick();

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( *System, system::type::call::FRAME_START );

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( *System, system::type::call::PRE_UPDATE );

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( *System, system::type::call::UPDATE );

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( *System, system::type::call::POST_UPDATE );

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( *System, system::type::call::FRAME_END );
	}

	void manager::Terminate( void ) noexcept
	{
		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( *System, system::type::call::TERMINATED );

		m_SystemMap.clear();
		m_Systems.clear();
	}
}