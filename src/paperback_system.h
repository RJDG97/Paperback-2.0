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

		void OnSystemCreated ( void ) noexcept {}	// When RegisterSystem is called - Manual Initialization
        void OnFrameStart    ( void ) noexcept {}	// At the start of the frame
        void PreUpdate       ( void ) noexcept {}	// Before Update()
		void Update		     ( void ) noexcept {}	// Recommended to use operator() if only performing a single loop - E.g. No comparing of entities against other entities
		void PostUpdate      ( void ) noexcept {}	// After Update()
		void OnFrameEnd      ( void ) noexcept {}	// At the end of the frame
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
		};

		enum class call
		{
			CREATED = 0
		,	FRAME_START
		,	PRE_UPDATE
		,	UPDATE
		,	POST_UPDATE
		,	FRAME_END
		,	TERMINATED
		};

		struct update
		{
			static constexpr auto id_v = id::UPDATE;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};

		struct info
		{
			using RunSystem  = void( system::instance&, type::call );
			using Destructor = void( system::instance& ) noexcept;

			const type::guid             m_Guid;
			const type::id               m_TypeID;
										 
			RunSystem*					 m_RunSystem;
			Destructor*					 m_Destructor;
			const char*					 m_pName{ };
		};
	}

	//-----------------------------------
	//           System Info
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
		void OnSystemCreated    ( void ) noexcept {}	// When RegisterSystem is called - Manual Initialization
        void OnFrameStart       ( void ) noexcept {}
        void PreUpdate          ( void ) noexcept {}
		void Update		        ( void ) noexcept {}
		void PostUpdate         ( void ) noexcept {}
		void OnFrameEnd         ( void ) noexcept {}
		void OnSystemTerminated ( void ) noexcept {}	// When Game is Terminated / Reloaded
	};

	struct instance : system_interface
	{
		// **Note:
		// The System merges "query" and the components listed in "operator()"
		// defined within the class, only iterating Archetypes that fit the
		// generated bitset
		using query = std::tuple<>;

		PPB_INLINE
		instance( coordinator::instance& Coordinator ) noexcept;
		instance( const instance& ) = delete;


		//-----------------------------------
		//    Extra System Functionality
		//-----------------------------------
		template < typename... T_COMPONENTS >
		archetype::instance& GetOrCreateArchetype( void ) noexcept;

		template< typename T_FUNCTION >
		void CreateEntity( T_FUNCTION&& Function ) noexcept;

		template< typename T_FUNCTION >
		void CreateEntities( T_FUNCTION&& Function
						   , const u32 Count ) noexcept;

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

		PPB_FORCEINLINE
		float DeltaTime() const noexcept;


		// Private This After
		paperback::coordinator::instance& m_Coordinator;
	};

	namespace details
	{
		template < typename USER_SYSTEM >
		struct completed final : USER_SYSTEM
		{
			PPB_INLINE
			completed( coordinator::instance& Coordinator ) noexcept;

			PPB_FORCEINLINE
			void Run( const paperback::system::type::call ) noexcept;
		};
	}
}