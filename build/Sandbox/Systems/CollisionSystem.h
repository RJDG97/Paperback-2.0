#pragma once

#include "Physics/geometry.h"
#include "Systems/DebugSystem.h"
#include "Physics/ResolveCollision.h"

struct collision_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "collision_system"
    };

    struct UnitTriggerEvent : paperback::event::instance< entity& , entity&, rigidforce&, rigidforce&> {};
    struct UnitTriggerStayEvent : paperback::event::instance< entity&, entity&, rigidforce&, rigidforce& > {};
    struct UnitTriggerExitEvent : paperback::event::instance< entity&, rigidforce& > {};

    using query = std::tuple< paperback::query::none_of<prefab> >;

    void operator()( paperback::component::entity& Entity, transform& Transform, rigidforce* RigidForce,  boundingbox * Boundingbox, sphere* Sphere, mass* m1,
        unitstate* state) noexcept
    {
        if ( Entity.IsZombie() ) return;
       
        // Initialize Query
        tools::query Query;

        Query.m_Must.AddFromComponents < transform >();
        Query.m_OneOf.AddFromComponents< boundingbox, sphere >();
        Query.m_NoneOf.AddFromComponents< prefab >();

        paperback::Vector3f tf = { Transform.m_Position.x + Transform.m_Offset.x, Transform.m_Position.y + Transform.m_Offset.y, Transform.m_Position.z + Transform.m_Offset.z };
        paperback::Vector3f xf;

        Boundingbox->m_Collided = false;

        ForEach( Search( Query ), [&]( paperback::component::entity& Dynamic_Entity, transform& Xform, rigidforce* RF, boundingbox* BB, sphere* Ball, mass* m2,
                                       unitstate* state2)  noexcept
        {
            if ( Entity.IsZombie() ) return;

            // Do not check against self
            if ((&Entity == &Dynamic_Entity) || (Dynamic_Entity.IsZombie())) return;

            // Add to collision map
            auto map = Boundingbox->m_CollisionState.find(Dynamic_Entity.m_GlobalIndex);
            if (map == Boundingbox->m_CollisionState.end()) {
                const auto& [map, Valid] = Boundingbox->m_CollisionState.insert({ Dynamic_Entity.m_GlobalIndex, false });
            }

            xf.x = Xform.m_Position.x + Xform.m_Offset.x;
            xf.y = Xform.m_Position.y + Xform.m_Offset.y;
            xf.z = Xform.m_Position.z + Xform.m_Offset.z;

            // Collision Detection
            if ( Boundingbox && BB)
            {
                if (AabbAabb(tf + Boundingbox->Min, tf + Boundingbox->Max, xf + BB->Min, xf + BB->Max))
                {
                    // boounce
                    if(RigidForce && RF && m1 && m2)
                        CheapaabbDynamic(Boundingbox, RigidForce, Transform, m1, BB, RF, Xform, m2);

                    //// On entry
                    //if(!Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex)) 
                    //    BroadcastGlobalEvent<UnitTriggerEvent>(Entity, Dynamic_Entity, *RigidForce, *RF);
                    //// Constant collision
                    //else
                    //    BroadcastGlobalEvent<UnitTriggerStayEvent>(Entity, Dynamic_Entity, *RigidForce, *RF);
                    
                    Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) = true;

                    Boundingbox->m_Collided = true;
                }
                else
                {
                    //// No longer colliding
                    //if( Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) )
                    //    BroadcastGlobalEvent<UnitTriggerExitEvent>(Entity, *RigidForce);

                    Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) = false;
                }
            }
            if (Sphere && Ball)
            {
                if (SphereSphere(tf, Sphere->m_Radius, xf, Ball->m_Radius))
                {
                    Sphere->m_Collided = Ball->m_Collided = true;
                }
                else
                    Sphere->m_Collided = Ball->m_Collided = false;
            }
        });
    }
};