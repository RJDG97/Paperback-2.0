#pragma once

namespace paperback::event
{
    //-----------------------------------
    //        Create Event Info
    //-----------------------------------

    namespace details
    {
        template< typename T_CLASS >
        consteval event::type::info CreateInfo( void ) noexcept
        {
            return event::type::info
            {
                .m_Guid                 = T_CLASS::typedef_v.m_Guid.m_Value
                                          ? T_CLASS::typedef_v.m_Guid
                                          : type::guid{ __FUNCSIG__ }
            ,   .m_ID                   = T_CLASS::typedef_v.id_v
            };
        }
    }

    //-----------------------------------
    //              Events
    //-----------------------------------

    template < typename...     T_ARGS >
    template < auto		       T_FUNCTION
			 , typename        T_SYSTEM >
    void instance<T_ARGS...>::RegisterEvent( T_SYSTEM* System ) noexcept
    {
        m_Events.push_back( info
                            {
                                /*
                                Operation - INVOKE( f, t1, t2, ..., tN )

                                    If f is a pointer to member function of class T:
                                    If std::is_base_of<T, std::decay_t<decltype(t1)>>::value is true, then INVOKE( f, t1, t2, ..., tN ) 
                                    is equivalent to - ( t1.*f )( t2, ..., tN )

                                    https://en.cppreference.com/w/cpp/utility/functional/invoke
                                */
                                .m_pEvent  = []( void* pSystem, T_ARGS&&... Args ) constexpr noexcept
                                             {
                                                 std::invoke( T_FUNCTION
                                                            , reinterpret_cast<T_SYSTEM*>( pSystem )
                                                            , std::forward<T_ARGS&&>( Args )... );
                                             }
                            ,   .m_pSystem = System
                            });
    }

    template < typename... T_ARGS >
    template < typename T_SYSTEM >
	void instance<T_ARGS...>::RemoveEvent( T_SYSTEM* System ) noexcept
    {
        for ( auto begin = m_Events.begin(), end = m_Events.end(); begin != end; ++begin )
        {
            if ( System == begin->m_pSystem )
            {
                m_Events.erase( begin );
                break;
            }
        }
    }

    template < typename... T_ARGS >
	void instance<T_ARGS...>::BroadcastEvent( T_ARGS&&... Args ) const noexcept
    {
        for ( const auto& Event : m_Events )
        {
            Event.m_pEvent( Event.m_pSystem
                          , std::forward<T_ARGS>( Args )... );
        }
    }
}