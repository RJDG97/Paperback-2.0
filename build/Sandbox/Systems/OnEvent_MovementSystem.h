#pragma once

#include "Systems/DebugSystem.h"
#include "Physics/ResolveCollision.h"

struct onevent_movement_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::system_event< collision_system, collision_system::UnitCollisionEvent >
    {
        .m_pName = "onevent_movement_system"
    };

    // EventReceiver does NOT call the operator() function
    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity) noexcept
    {
        // operator() will not be called for Systems declared as system::type::system_event
    }

    void OnEvent(rigidforce& RF1, rigidforce& RF2) noexcept
    {
        RF1.m_Forces = { 0.0f, 0.0f, 0.0f };
        RF1.m_Momentum = { 0.0f, 0.0f, 0.0f };

        RF2.m_Forces = { 0.0f, 0.0f, 0.0f };
        RF2.m_Momentum = { 0.0f, 0.0f, 0.0f };
    }
};