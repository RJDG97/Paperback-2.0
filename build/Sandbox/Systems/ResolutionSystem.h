#pragma once

#include "Physics/geometry.h"
#include "Systems/DebugSystem.h"
#include "Physics/ResolveCollision.h"

struct resolution_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "resolution_system"
    };

    // std::unordered_map<> -> link to oobjs
    // std::vector<list of all colliders/collision objs>

    // events add to the list. this simply resolves all at once
};