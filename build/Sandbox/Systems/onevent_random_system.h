#pragma once

struct onevent_random_system : paperback::system::instance
{
    
    /* FOR SYSTEM EVENTS */
    // system::type::system_event< dispatcher_system, dispatcher_system_event_instance_type >
    // constexpr static auto typedef_v = paperback::system::type::system_event< random_system, random_system::test_event >
    // {
    //     .m_pName = "onevent_random_system"
    // };

    /* NORMAL SYSTEM REGISTRATION */
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "onevent_random_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        //RegisterGlobalEventClass<random_system::test>( this );
    }

    // event_system does NOT call the operator() function, 
    PPB_FORCEINLINE
    void operator()( paperback::component::entity& Entity ) noexcept
    {

    }

    // Can be used by either Global / System Events
    void OnEvent( std::string& abc ) noexcept
    {
        INFO_PRINT( abc.c_str() );
    }
};



//struct global_event
//{
//    //using test = paperback::event::instance<std::string>;
//
//    struct test : paperback::event::instance<std::string&>
//    {};
//
//    global_event()
//    {
//        //PPB.RegisterEvent<test>();
//        PPB.RegisterEventClass<test>( this );
//    }
//        
//    // event_system receives events and calls the OnEvent function
//    void OnEvent( std::string abc ) noexcept
//    {
//        INFO_PRINT( abc.c_str() );
//    }
//};

