#pragma once

struct random_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "random_system"
    };

    // Declare an Event Instance
    // Specify argument types inside of Template Parameters
    using test_event = paperback::event::instance<std::string>;

    // Declare a "list" of Events
    using events = std::tuple< test_event >;

    PPB_FORCEINLINE
    void operator()( paperback::component::entity& Entity ) noexcept
    {
        BroadcastEvent<test_event>( this, "Broadcasting event from random_system" );
    }
};