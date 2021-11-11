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

    struct UnitTriggerEvent : paperback::event::instance< entity& , rigidforce&> {};
    struct UnitTriggerStayEvent : paperback::event::instance< entity&, entity& > {};
    struct UnitTriggerExitEvent : paperback::event::instance< entity&, rigidforce& > {};

    struct UnitWaypointEvent : paperback::event::instance< waypointUserv1&, waypointv1&, rigidforce& > {};

    void operator()( paperback::component::entity& Entity, transform& Transform, /*rigidbody& rb,*/rigidforce* RigidForce,  boundingbox * Boundingbox, sphere* Sphere, collidable* col1,
        waypointUserv1* wu1, waypointv1* wp1) noexcept
    {
        if ( Entity.IsZombie() ) return;

        // Initialize Query
        tools::query Query;
        Query.m_Must.AddFromComponents < transform >();
        Query.m_OneOf.AddFromComponents< boundingbox, sphere >();

        paperback::Vector3f tf = { Transform.m_Position.x + Transform.m_Offset.x, Transform.m_Position.y + Transform.m_Offset.y, Transform.m_Position.z + Transform.m_Offset.z };
        paperback::Vector3f xf;

        ForEach( Search( Query ), [&](paperback::component::entity& Dynamic_Entity, transform& Xform, rigidforce* RF, boundingbox* BB, sphere* Ball, collidable* col2,
            waypointUserv1* wu2, waypointv1* wp2) noexcept -> bool
            {
                assert(Entity.IsZombie() == false);

                // Do not check against self
                if ( (&Entity == &Dynamic_Entity) || (Dynamic_Entity.IsZombie()) ) return false;

                xf.x = Xform.m_Position.x + Xform.m_Offset.x;
                xf.y = Xform.m_Position.y + Xform.m_Offset.y;
                xf.z = Xform.m_Position.z + Xform.m_Offset.z;

                // Collision Detection
                if ( Boundingbox && BB)
                {
                    if (col1 && col2)
                    {
                        // Only checks if it has a layer? -> Nope, it checks if they have same layer
                        if (col1->m_CollidableLayers.Has(col2->m_CollisionLayer)) 
                        {
                            // added to parameters
                            if (AabbAabb(tf + Boundingbox->Min, tf + Boundingbox->Max, xf + BB->Min, xf + BB->Max))
                            {
                                if (RigidForce && RF)
                                {
                                    bool checker = CheapaabbDynamic(
                                        Boundingbox,
                                        RigidForce,
                                        Transform,
                                        BB,
                                        RF,
                                        Xform);
                                }

                                // if both waypoint users collides
                                if (wu1 && wu2)
                                {
                                    if(!wu1->isAttacking && RigidForce != nullptr)
                                        BroadcastGlobalEvent<UnitTriggerEvent>(Entity, *RigidForce);
                                    else
                                        BroadcastGlobalEvent<UnitTriggerStayEvent>(Entity, Dynamic_Entity);
                                    wu1->isAttacking = true;
                                }

                                // unit collide with waypoint
                                //else if
                                if (wu1 && wp2){
                                    if (wp2->isBase) {
                                        if(wu1->isAttacking)
                                            BroadcastGlobalEvent<UnitTriggerStayEvent>(Entity, Dynamic_Entity);
                                        else
                                            BroadcastGlobalEvent<UnitTriggerEvent>(Entity, *RigidForce);
                                        wu1->isAttacking = true;
                                    }
                                    else
                                        BroadcastGlobalEvent<UnitWaypointEvent>(*wu1, *wp2, *RigidForce);
                                }
                                //else if (wu2 && wp1){
                                //    if (wp2->isBase) {
                                //        if (wu1->isAttacking)
                                //            BroadcastGlobalEvent<UnitTriggerStayEvent>(Entity, Dynamic_Entity);
                                //        else
                                //            BroadcastGlobalEvent<UnitTriggerEvent>(Entity, *RigidForce);
                                //        wu1->isAttacking = true;
                                //    }
                                //    else
                                //        BroadcastGlobalEvent<UnitWaypointEvent>(*wu2, *wp1, *RigidForce);
                                //}
                                Boundingbox->m_Collided = BB->m_Collided = true;
                            }
                            else {
                                if (wu1 && wu1->isAttacking && Boundingbox->m_Collided && RigidForce) {
                                    BroadcastGlobalEvent<UnitTriggerExitEvent>(Entity, *RigidForce);
                                    wu1->isAttacking = false;
                                }
                                Boundingbox->m_Collided = BB->m_Collided = false;
                            }
                        }
                    }
                }
                if (Sphere && Ball)
                {
                    // added to parameters
                    if (SphereSphere(tf, Sphere->m_fRadius, xf, Ball->m_fRadius))
                    {
                        Sphere->m_Collided = Ball->m_Collided = true;
                    }
                    else
                        Sphere->m_Collided = Ball->m_Collided = false;
                }
                
                /* Return true on deletion of collided entities */
                //return false;
            });
    }
};