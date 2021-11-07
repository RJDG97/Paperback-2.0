#pragma once

struct onevent_random_system : paperback::system::instance
{
    // system::type::system_event< dispatcher_system, dispatcher_system_event_instance_type >
    constexpr static auto typedef_v = paperback::system::type::system_event< random_system, random_system::test_event >
    {
        .m_pName = "random_system"
    };

    // event_system does NOT call the operator() function, 
    PPB_FORCEINLINE
    void operator()( paperback::component::entity& Entity ) noexcept
    {
        INFO_PRINT( "onevent_random_system loop" );
    }

    // event_system receives events and calls the OnEvent function
    void OnEvent( std::string abc ) noexcept
    {
        INFO_PRINT( abc.c_str() );
    }
};