#pragma once

namespace paperback::event
{
    //-----------------------------------
    //            Register
    //-----------------------------------

    template < paperback::concepts::Event T_EVENT >
    void manager::RegisterEvent( void ) noexcept
    {
        m_GlobalEvents.push_back( std::make_unique<T_EVENT>() );
        m_GlobalEventsMap.emplace( event::info_v<T_EVENT>.m_Guid
                                 , m_GlobalEvents.back().get() );
    }

    template < paperback::concepts::Event T_EVENT
             , typename                   T_CLASS >
    void manager::RegisterEventClass( T_CLASS* Class ) noexcept
    {
        auto it = m_GlobalEventsMap.find( event::info_v<T_EVENT>.m_Guid );

        // If Event Was Not Registered Yet
        if ( it == m_GlobalEventsMap.end() )
        {
            RegisterEvent<T_EVENT>();
            it = m_GlobalEventsMap.find( event::info_v<T_EVENT>.m_Guid );
        }

        static_cast<T_EVENT*>( it->second )->RegisterEvent< &T_CLASS::OnEvent >( Class );
    }


    //-----------------------------------
    //             Remove
    //-----------------------------------

    template < paperback::concepts::Event T_EVENT >
    void manager::RemoveEvent( void ) noexcept
    {
        //for ( auto begin = m_GlobalEvents.begin(), end = m_GlobalEvents.end(); begin != end; ++begin )
        //{

        //}
    }

    template < paperback::concepts::Event T_EVENT
             , typename                   T_CLASS >
    void manager::RemoveEventClass( T_CLASS* Class ) noexcept
    {
        auto it = m_GlobalEventsMap.find( event::info_v<T_EVENT>.m_Guid );

        if ( it != m_GlobalEventsMap.end() )
        {
            static_cast<T_EVENT*>( it->second )->RemoveEvent( static_cast<T_CLASS*>( Class ) );
        }
    }


    //-----------------------------------
    //            Broadcast
    //-----------------------------------

    template < paperback::concepts::Event T_EVENT
             , typename...                T_ARGS >
    void manager::BroadcastEvent( T_ARGS&&... Args ) const noexcept
    {
        auto it = m_GlobalEventsMap.find( event::info_v<T_EVENT>.m_Guid );

        if ( it == m_GlobalEventsMap.end() )
        {
            WARN_LOG( "Event Manager: Event Is Not Registered" );
            return;
        }

        static_cast<T_EVENT*>( it->second )->BroadcastEvent( std::forward< T_ARGS&& >( Args )... );
    }


    manager::~manager( void ) noexcept
    {
        m_GlobalEventsMap.clear();
        [&]()
        {
            GlobalEvents e;
            std::swap( e, m_GlobalEvents );
        }();
    }
}