#pragma once

namespace paperback::event
{
    struct manager
    {
        using GlobalEvents   = std::vector< std::unique_ptr<paperback::event::event_interface> >;
        using GlobalEventMap = std::unordered_map< paperback::event::type::guid
                                                 , paperback::event::event_interface* >;

        PPB_INLINE
        ~manager( void ) noexcept;

        //-----------------------------------
        //            Register
        //-----------------------------------

        template < paperback::concepts::Event T_EVENT >
        void RegisterEvent( void ) noexcept;

        template < paperback::concepts::Event T_EVENT
                 , typename                   T_CLASS >
        void RegisterEventClass( T_CLASS* Class ) noexcept;


        //-----------------------------------
        //             Remove
        //-----------------------------------

        template < paperback::concepts::Event T_EVENT >
        void RemoveEvent( void ) noexcept;

        template < paperback::concepts::Event T_EVENT
                 , typename                   T_CLASS >
        void RemoveEventClass( T_CLASS* Class ) noexcept;


        //-----------------------------------
        //            Broadcast
        //-----------------------------------

        template < paperback::concepts::Event T_EVENT
                 , typename...                T_ARGS >
        void BroadcastEvent( T_ARGS&&... Args ) const noexcept;


        GlobalEvents       m_GlobalEvents;
        GlobalEventMap     m_GlobalEventsMap;
    };
}