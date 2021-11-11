#pragma once

#include "Systems/DebugSystem.h"

struct onevent_WaypointTrigger_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "onevent_waypointtrigger_system"
    };

    PPB_FORCEINLINE
        void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<collision_system::UnitWaypointEvent>(this);
    }

    // EventReceiver does NOT call the operator() function
    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity) noexcept
    {
        // operator() will not be called for Systems declared as system::type::system_event
    }

    void OnEvent(waypointUserv1& obj, waypointv1& wp, rigidforce& rf) noexcept
    {
            obj.m_destination = wp.m_destination[obj.m_player];
            // be careful with this!! set speed if there is one
            if (wp.b_enableSpeed)
            {
                // first var is the user
                rf.m_Momentum = wp.m_setspeed[obj.m_player] * rf.m_Mass;
            }
    }
};