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

    struct UnitTriggerEvent : paperback::event::instance< entity& , entity&, rigidforce&, rigidforce& > {};
    struct UnitTriggerStayEvent : paperback::event::instance< entity&, entity&, rigidforce&, rigidforce&, bool& > {};
    struct UnitTriggerExitEvent : paperback::event::instance< entity&, rigidforce&, bool& > {};

    using query = std::tuple
    <
        paperback::query::none_of<prefab>
    >;

    void operator()( paperback::component::entity& Entity, transform& Transform, rigidforce* RigidForce,  boundingbox * Boundingbox, sphere* Sphere, mass* m1 ) noexcept
    {
        if ( Entity.IsZombie() ) return;
       
        // Initialize Query
        bool SkipUnit = false;
        tools::query Query;

        Query.m_Must.AddFromComponents < transform >();
        Query.m_OneOf.AddFromComponents< boundingbox, sphere >();
        Query.m_NoneOf.AddFromComponents< prefab >();

        Boundingbox->m_Collided = false;

        ForEach( Search( Query ), [&]( paperback::component::entity& Dynamic_Entity, transform& Xform, rigidforce* RF, boundingbox* BB, sphere* Ball, mass* m2 )  noexcept
        {
            if ( Entity.IsZombie() ) return;

            // Do not check against self
            if ((&Entity == &Dynamic_Entity) || (Dynamic_Entity.IsZombie())) return;

            // Add to collision map
            auto map = Boundingbox->m_CollisionState.find(Dynamic_Entity.m_GlobalIndex);
            if (map == Boundingbox->m_CollisionState.end()) {
                const auto& [map, Valid] = Boundingbox->m_CollisionState.insert({ Dynamic_Entity.m_GlobalIndex, false });
            }

            // Collision Detection
            if ( Boundingbox && BB)
            {
                // If Both Entities Are Colliding Already
                if ( AabbAabb( Transform.m_Position + Boundingbox->Min, Transform.m_Position + Boundingbox->Max
                             , Xform.m_Position + BB->Min, Xform.m_Position + BB->Max ) )
                {
                    // Update Collision State of Current Entity to Other Entity
                    Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) = true;
                    Boundingbox->m_Collided = BB->m_Collided = true;

                    // Current Entity is NOT Colliding with Other Entity
                    if( !Boundingbox->m_CollisionState.at( Dynamic_Entity.m_GlobalIndex ) )
                        BroadcastGlobalEvent<UnitTriggerEvent>( Entity, Dynamic_Entity, *RigidForce, *RF );
                    // Current Entity is ALREADY Colliding with Other Entity
                    else
                        BroadcastGlobalEvent<UnitTriggerStayEvent>( Entity, Dynamic_Entity, *RigidForce, *RF, SkipUnit );

                    // Collision Response
                    if ( RigidForce && RF && !SkipUnit /*&& m1 && m2*/ )
                        CheapaabbDynamic( Boundingbox, RigidForce, Transform, m1, BB, RF, Xform, m2 );
                }
                else
                {
                    // No longer colliding
                    if( Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) )
                        BroadcastGlobalEvent<UnitTriggerExitEvent>( Entity, *RigidForce, SkipUnit );

                    Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) = false;
                }
            }
            if (Sphere && Ball)
            {
                if ( SphereSphere( Transform.m_Position, Sphere->m_Radius, Xform.m_Position, Ball->m_Radius) )
                {
                    Sphere->m_Collided = Ball->m_Collided = true;
                }
                else
                    Sphere->m_Collided = Ball->m_Collided = false;
            }
        });
    }
};