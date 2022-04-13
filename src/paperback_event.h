/**********************************************************************************
*\file         paperback_event.h
*\brief        paperback_event.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace paperback::event
{
	//-----------------------------------
	//           Event Info
	//-----------------------------------

	struct event_interface;

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
			const type::guid	      m_Guid;
			const event::type::id	  m_ID;
		};


        //-----------------------------------
	    //           Event Types
	    //-----------------------------------

        struct global
        {
            const type::guid      m_Guid       { };
            static constexpr auto id_v       = event::type::id::GLOBAL;

        };

        struct post
        {
            const type::guid        m_Guid       { };
            static constexpr auto   id_v       = event::type::id::POST;
        };
	}


	//-----------------------------------
	//        Create Event Info
	//-----------------------------------

	namespace details
    {
        template< typename T_CLASS >
        consteval event::type::info CreateInfo( void ) noexcept;

        template< typename T_CLASS >
        static constexpr auto info_v = event::details::CreateInfo<T_CLASS>();
    }

    template< typename T_CLASS >
    constexpr auto& info_v = event::details::info_v< T_CLASS >;


	//-----------------------------------
	//          Event Instance
	//-----------------------------------

	struct event_interface
	{
		constexpr static auto typedef_v = paperback::event::type::global {};
		virtual ~event_interface() = default;
	};

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
		virtual ~instance() = default;


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