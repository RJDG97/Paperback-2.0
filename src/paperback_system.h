#pragma once

/*
To Use:

	**Note:
	- For short functions, use PPB_INLINE

	1. Create a System:
		
		struct my_system : paperback::system::instance
		{
			constexpr static auto typedef_v = paperback::system::type::update
			{
				.m_pName = "my_system"
			};
		};


	2. Define System Main Loop
		
		// 1. Either use operator() - Parameters are the Components required for each system update loop on a SINGLE entity - O( 1 )

		PPB_INLINE
		void operator()( paperback::component::entity& Entity, Transform& transform, Timer& timer, Bullet& bullet ) const noexcept
		{
			// System Update Code - FOR A SINGLE ENTITY
		}

		// 2. Or use a manual update - This is mainly used where more controlled iteration is required - E.g. Collision Checking

		PPB_INLINE
		void Update( void )
		{
			// System Update Code - Can be for a Single Archetype / N^2 Loop
		}


	3. Register System - Order of registering matters
		
		PPB.RegisterSystems<
			my_system
		>();


	4. More Advanced Iteration - Place this code inside the System Loop ( Either operator() or Update() )
		
		// tools::query::m_Must   - The Archetype that is selected MUST have all Components listed within the tuple ( Parameter Type = & )
		// tools::query::m_OneOf  - The Archetype that is selected MUST have AT LEAST ONE of the Components listed within the tuple ( Parameter Type = * ) - Not recommended unless necessary (Single purpose systems)
		// tools::query::m_NoneOf - The Archetype that is selected MUST have NONE of the Components listed within the tuple

		tools::query Query;
        Query.m_Must.AddFromComponents< Transform >();

		// Return bool for the lambda terminates the ForEach loop on a "return true" statement
		// 2 Variants of ForEach, one iterates all Entities within the the Archetypes returned from Search( Query ), other terminates on "return true"
        m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( paperback::component::entity& Entity, transform& Transform ) noexcept -> bool
        {
			
		}


	5. More Advanced Update Handling - Define these functions within your System ( Use PPB_INLINE )

		void OnSystemCreated    ( void ) noexcept {}	// When RegisterSystem is called - Manual Initialization
        void OnFrameStart       ( void ) noexcept {}	// At the start of the frame
        void PreUpdate          ( void ) noexcept {}	// Before Update()
		void Update		        ( void ) noexcept {}	// Recommended to use operator() if only performing a single loop - E.g. No comparing of entities against other entities
		void PostUpdate         ( void ) noexcept {}	// After Update()
		void OnFrameEnd         ( void ) noexcept {}	// At the end of the frame
		void OnSystemTerminated ( void ) noexcept {}	// When Game is Terminated / Reloaded
*/

namespace paperback::system
{
	//-----------------------------------
	//          System Types
	//-----------------------------------

	namespace type
	{
		using guid = xcore::guid::unit<64, struct system_tag>;

		enum class id : u8
		{
			UPDATE = 0
		,	SYSTEM_EVENT
		,	GLOBAL_SYSTEM_EVENT
		,	NOTIFY_PREFAB_MODIFIED		// Future Implementation
		,	NOTIFY_COMPONENT_ADDED		// Future Implementation
		,	NOTIFY_COMPONENT_REMOVED	// Future Implementation
		};

		enum class call
		{
			CREATED = 0
		,	FRAME_START
		,	PRE_UPDATE
		,	UPDATE
		,	POST_UPDATE
		,	FRAME_END
		,	STATECHANGED
		,	TERMINATED
		};
		

		//-----------------------------------
		//          System Types
		//-----------------------------------

		struct update
		{
			static constexpr auto id_v = id::UPDATE;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};

		struct global_system_event
		{
			static constexpr auto id_v = id::GLOBAL_SYSTEM_EVENT;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};

		template < typename T_SYSTEM
				 , typename T_EVENT >
		requires( std::derived_from< T_SYSTEM, paperback::system::instance>
             && ( xcore::types::is_specialized_v< paperback::event::instance, T_EVENT > ))
		struct system_event
		{
			using SystemType = T_SYSTEM;
			using EventType  = T_EVENT;

			static constexpr auto id_v = id::SYSTEM_EVENT;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};

		struct notify_prefab_modified
		{
			static constexpr auto id_v = id::NOTIFY_PREFAB_MODIFIED;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};

		struct notify_component_added
		{
			static constexpr auto id_v = id::NOTIFY_COMPONENT_ADDED;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};

		struct notify_component_removed
		{
			static constexpr auto id_v = id::NOTIFY_COMPONENT_REMOVED;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};


		//-----------------------------------
		//          System Info
		//-----------------------------------

		struct info
		{
			using RunSystem  = void( system::instance&, type::call );
			using Destructor = void( system::instance& ) noexcept;

			const type::guid             m_Guid;
			const type::id               m_TypeID;
										 
			Destructor*					 m_Destructor;
			mutable tools::query		 m_Query;
			const char*					 m_pName{ };
		};
	}

	//-----------------------------------
	//        Create System Info
	//-----------------------------------

	namespace details
    {
        template< typename T_SYSTEM >
        consteval system::type::info CreateInfo( void ) noexcept;

        template< typename T >
        static constexpr auto info_v = system::details::CreateInfo<T>();
    }

    template< typename T_SYSTEM >
    constexpr auto& info_v = details::info_v< std::decay_t<T_SYSTEM> >;


	//-----------------------------------
	//        System Interface
	//-----------------------------------

	struct system_interface
	{
		// **Note:
		// The System merges "query" and the components listed in "operator()"
		// defined within the class, only iterating Archetypes that fit the
		// generated bitset
		using entity = paperback::component::entity;
		using query  = std::tuple<>;
		using events = std::tuple<>;

		// Main System Loops
		void OnSystemCreated    ( void )        noexcept {}				// Executed on RegisterSystem()
        void OnFrameStart       ( void )        noexcept {}				// Executed on System Run - 1
        void PreUpdate          ( void )        noexcept {}				// Executed on System Run - 2
		void Update		        ( void )        noexcept {}				// Executed on System Run - 3
		void PostUpdate         ( void )        noexcept {}				// Executed on System Run - 4
		void OnFrameEnd         ( void )        noexcept {}				// Executed on System Run - 5
		void OnSystemTerminated ( void )        noexcept {}				// Executed on Program Termination
		void OnStateChange		( void )        noexcept {}				// Executed on Scene Change - LoadScene

		// Event						        
		void OnEvent            ( ... )         noexcept {}				// System Event - Override with required parameters
		void OnDebug            ( const bool& ) noexcept {}				// Executed on SystemMgr's "m_Events.m_OnDebug.BroadcastEvent( Status );"
		void OnPause			( const bool& ) noexcept {}				// Executed on SystemMgr's "m_Events.m_OnPause.BroadcastEvent( Status );"


		bool m_bPaused = false;
		bool m_bDebug  = false;
	};


	//-----------------------------------
	//    System Instance - Default
	//-----------------------------------

	struct instance : system_interface
	{
		PPB_INLINE
		instance( coordinator::instance& Coordinator ) noexcept;
		instance( const instance& ) = delete;


		//-----------------------------------
		//    Extra System Functionality
		//-----------------------------------

		template < typename T_EVENT >
        void RegisterGlobalEvent( void ) noexcept;

        template < paperback::concepts::Event T_EVENT
                 , typename                   T_CLASS >
        void RegisterGlobalEventClass( T_CLASS* Class ) noexcept;

		template < paperback::concepts::Event T_EVENT >
        void RemoveGlobalEvent( void ) noexcept;

        template < paperback::concepts::Event T_EVENT
                 , typename                   T_CLASS >
        void RemoveGlobalEventClass( T_CLASS* Class ) noexcept;

		template < typename... T_COMPONENTS >
		archetype::instance& GetOrCreateArchetype( void ) noexcept;

		template< typename T_FUNCTION >
		void CreateEntity( T_FUNCTION&& Function ) noexcept;

		template< typename T_FUNCTION >
		void CreateEntities( T_FUNCTION&& Function
						   , const u32 Count ) noexcept;

		template < typename       T_EVENT
			     , typename       T_SYSTEM
			     , typename...    T_ARGS >
		requires ( std::derived_from< T_SYSTEM, paperback::system::instance >
			  && ( !std::is_same_v< typename T_SYSTEM::events, paperback::system::system_interface::events > ))
		void BroadcastEvent( T_SYSTEM* System, T_ARGS&&... Args ) const noexcept;

		template < paperback::concepts::Event T_EVENT
                 , typename...                T_ARGS >
        void BroadcastGlobalEvent( T_ARGS&&... Args ) const noexcept;

		PPB_INLINE
        paperback::entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

		PPB_INLINE
		void DeleteEntity( component::entity& Entity ) noexcept;

		template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search() const noexcept;

		PPB_INLINE
        std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;

		template < concepts::Callable_Void T_FUNCTION >
        void ForEach( const std::vector<archetype::instance*>& ArchetypeList
					, T_FUNCTION&& Function ) noexcept;

        template < concepts::Callable_Bool T_FUNCTION >
        void ForEach( const std::vector<archetype::instance*>& ArchetypeList
					, T_FUNCTION&& Function ) noexcept;

		template< typename T_SYSTEM >
		T_SYSTEM* FindSystem( void ) noexcept;

		template< typename T_SYSTEM >
		T_SYSTEM& GetSystem( void ) noexcept;

		template < paperback::concepts::Script T_SCRIPT >
        T_SCRIPT* FindScript( const paperback::u64 ScriptGuid ) const noexcept;

        template < paperback::concepts::Script T_SCRIPT >
        T_SCRIPT& GetScript( const paperback::u64 ScriptGuid ) const noexcept;

		PPB_INLINE
		glm::vec3 GetMousePosition() noexcept;

		PPB_INLINE
		glm::vec2 GetMouseDirection() noexcept;

		PPB_INLINE
		glm::vec2 GetMousePositionInUI() noexcept;

		PPB_INLINE
		glm::vec3 GetViewportMousePosition( glm::vec2 viewport_min, glm::vec2 viewport_max ) noexcept;

		PPB_FORCEINLINE
		float DeltaTime() const noexcept;


		paperback::coordinator::instance& m_Coordinator;
	};


	//-----------------------------------
	//    System Instance - Pausable
	//-----------------------------------

	struct pausable_instance : instance
	{
		void OnPause( const bool& Status ) noexcept { m_bPaused = Status; }
	};


	//-----------------------------------
	//          System Wrapper
	//-----------------------------------

	namespace details
	{
		template < typename USER_SYSTEM >
		struct completed final : USER_SYSTEM
		{
			PPB_INLINE
			completed( coordinator::instance& Coordinator ) noexcept;

			PPB_INLINE
			void System_OnFrameStart( void ) noexcept;

			PPB_INLINE
			void System_OnPreUpdate( void ) noexcept;

			PPB_INLINE
			void System_OnUpdate( void ) noexcept;

			PPB_INLINE
			void System_OnPostUpdate( void ) noexcept;

			PPB_INLINE
			void System_OnFrameEnd( void ) noexcept;

			USER_SYSTEM::events   m_Events;
		};
	}
}