#pragma once

namespace paperback::system
{
	//-----------------------------------
	//          System Manager
	//-----------------------------------
	class manager final
	{
	public:

		using SystemMap  = std::unordered_map< system::type::guid, system::instance* >;
		using SystemList = std::vector< std::pair< const system::type::info*, std::unique_ptr<system::instance> > >;

		manager( tools::clock& Clock );
		manager( const manager& ) = delete;
		~manager();

		template < typename... T_SYSTEMS >
		constexpr void RegisterSystems( coordinator::instance& Coordinator ) noexcept;

		template < typename T_SYSTEM >
		constexpr T_SYSTEM& RegisterSystem( coordinator::instance& Coordinator ) noexcept;

		template < typename T_SYSTEM >
		T_SYSTEM* FindSystem( void ) noexcept;

		PPB_INLINE
		void Run( void ) noexcept;

		PPB_INLINE
		void Terminate( void ) noexcept;


	private:

		tools::clock&			m_SystemClock;
		SystemMap				m_SystemMap;
		SystemList				m_Systems;
	};
}