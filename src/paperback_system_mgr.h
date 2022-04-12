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

		struct SystemEvents
		{
			paperback::event::instance<> m_OnPreStatusUpdate;
			paperback::event::instance<> m_OnFrameStart;
			paperback::event::instance<> m_OnPreUpdate;
			paperback::event::instance<> m_OnUpdate;
			paperback::event::instance<> m_OnPostUpdate;
			paperback::event::instance<> m_OnFrameEnd;
			paperback::event::instance<> m_OnSystemTerminated;
			paperback::event::instance<> m_OnStateChange;
			paperback::event::instance<> m_OnStateLoad;
			paperback::event::instance<> m_OnStateQueued;
			paperback::event::instance<const bool&> m_OnDebug;
			paperback::event::instance<const bool&> m_OnPause;
		};

		//-----------------------------------
		//     Constructor / Destructor
		//-----------------------------------

		PPB_INLINE
		manager( tools::clock& Clock );

		PPB_INLINE
		manager( const manager& ) = delete;

		PPB_INLINE
		~manager();


		//-----------------------------------
		//            Systems
		//-----------------------------------

		template < typename... T_SYSTEMS >
		constexpr void RegisterSystems( coordinator::instance& Coordinator ) noexcept;

		template < typename T_SYSTEM >
		constexpr T_SYSTEM& RegisterSystem( coordinator::instance& Coordinator ) noexcept;

		template < typename T_SYSTEM >
		T_SYSTEM* FindSystem( void ) noexcept;


		//-----------------------------------
		//             Updates
		//-----------------------------------

		PPB_INLINE
		void Run( void ) noexcept;

		PPB_INLINE
		void ToggleDebug( const bool& Status ) noexcept;

		PPB_INLINE
		void TogglePause( const bool& Status ) noexcept;

		PPB_INLINE
		void PrepareSystemsReset( void ) noexcept;

		PPB_INLINE
		void ResetSystems( void ) noexcept;

		PPB_INLINE
		void ReloadSystems( void ) noexcept;

		PPB_INLINE
		void Terminate( void ) noexcept;


	private:

		//-----------------------------------
		//   Registration Helper Functions
		//-----------------------------------

		template < paperback::concepts::System T_SYSTEM >
		void InitializeSystemQuery( void ) noexcept;

		template < paperback::concepts::System T_SYSTEM >
		void InitializeSystemUpdateEvents( T_SYSTEM* System ) noexcept;


		tools::clock&			m_SystemClock;
		SystemMap				m_SystemMap;
		SystemList				m_Systems;
		SystemEvents			m_Events;
	};
}