#pragma once

/*
To Use:

	1. Create an Event:

		struct RandomEvent : paperback::event::instance
		{
			RandomEvent( paperback::component::entity Entity ) : m_Entity{ Entity } {}
			Paperback::EntityID m_Entity;
		};


	2. Create a public handler function within the System handling the Event

		void System::TestRandomEvent( std::shared_ptr< RandomEvent > Event )
		{
			DEBUG_PRINT("Random Event");

			// To access data ( Probably making a macro for this in the near future )
			static_cast< RandomEvent* >( *Event )->m_Entity;
		}


	3. Register Event inside OnCreate() - Subscribe Event Function has not been incorporated into Coordinator yet

		Action< std::shared_ptr< RandomEvent > > Event_1 = PPB_BIND( System::TestRandomEvent );
		m_Coordinator.SubscribeEvent( Event_1 );


	4. Publish Event

		m_Coordinator.PublishEvent< RandomEvent >( some_random_entity_component );
*/

namespace paperback::event
{
	//-----------------------------------
	//           Event Info
	//-----------------------------------

	namespace type
	{
		using guid = xcore::guid::unit<64, struct event_tag>;

		enum class id : std::uint8_t
        {
            GLOBAL = 0		  // Global Event
        ,   POST			  // Post Frame Event
        };

		struct info
		{
			type::guid	      m_Guid;
			const char*       m_pName;
		};


        //-----------------------------------
	    //           Event Types
	    //-----------------------------------

        struct global
        {
            static constexpr auto id_v       = id::GLOBAL;

            type::guid                         m_Guid { };
            const char*                        m_pName{ };
        };

        struct post
        {
            static constexpr auto id_v       = id::POST;

            type::guid                         m_Guid { };
            const char*                        m_pName{ };
        };
	}


	//-----------------------------------
	//        Create Event Info
	//-----------------------------------

	namespace details
    {
        template< typename T_SYSTEM >
        consteval event::type::info CreateInfo( void ) noexcept;

        template< typename T >
        static constexpr auto info_v = event::details::CreateInfo<T>();
    }

    template< typename T_SYSTEM >
    constexpr auto& info_v = details::info_v< paperback::BaseType<T_SYSTEM> >;


	//-----------------------------------
	//          Event Instance
	//-----------------------------------

	struct event_interface {};

	template < typename... T_ARGS >
	struct instance : event_interface
	{
		struct info
		{
			using Callback = void( void*, T_ARGS&&... );

			Callback*         m_pEvent;
			void*		      m_pSystem;
		};

		//-----------------------------------
		//           Constructors
		//-----------------------------------

		instance( void ) noexcept = default;
		instance( const instance& ) noexcept = delete;


		//-----------------------------------
		//              Events
		//-----------------------------------

		template < auto		T_FUNCTION
				 , typename T_SYSTEM >
		void RegisterEvent( T_SYSTEM* System ) noexcept;

		template < typename T_SYSTEM >
		void RemoveEvent( T_SYSTEM* System ) noexcept;

		PPB_INLINE
		void BroadcastEvent( T_ARGS&&... Args ) const noexcept;


		std::vector<info>     m_Events;
	};
}