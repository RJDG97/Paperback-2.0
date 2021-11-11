#pragma once

#include "Physics/geometry.h"
#include "Systems/DebugSystem.h"
#include "Physics/ResolveCollision.h"

// waypoint assigning should be after collision system and before physics update (so that it doesn't overshot the point)

struct waypoint_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "waypoint_system"
    };

    void operator()( paperback::component::entity& Entity, transform& Transform, boundingbox * Boundingbox, sphere* Sphere, Collidable* col1) noexcept
    {
        if ( Entity.IsZombie() ) return;

    }
};