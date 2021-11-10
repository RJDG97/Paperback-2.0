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
    struct UnitTriggerStayEvent : paperback::event::instance< entity& > {};
    struct UnitTriggerExitEvent : paperback::event::instance< entity&, rigidforce& > {};

    void operator()( paperback::component::entity& Entity, transform& Transform, /*rigidbody& rb,*/rigidforce* RigidForce,  boundingbox * Boundingbox, sphere* Sphere, collidable* col1) noexcept
    {
        if ( Entity.IsZombie() ) return;

        // Initialize Query
        tools::query Query;
        Query.m_Must.AddFromComponents < transform >();
        Query.m_OneOf.AddFromComponents< boundingbox, sphere >();

        paperback::Vector3f tf = { Transform.m_Position.x + Transform.m_Offset.x, Transform.m_Position.y + Transform.m_Offset.y, Transform.m_Position.z + Transform.m_Offset.z };
        paperback::Vector3f xf;

        ForEach( Search( Query ), [&](paperback::component::entity& Dynamic_Entity, transform& Xform, rigidforce* RF, boundingbox* BB, sphere* Ball, collidable* col2) noexcept -> bool
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
                        if (col1->m_CollidableLayers.Has(col2->m_CollisionLayer)) // Only checks if it has a layer?
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
                                // Check if layer is unit
                                    // If first instance of collision (may be an issue with gravity)
                                    if (!Boundingbox->m_Collided)
                                        BroadcastGlobalEvent<UnitTriggerEvent>(Entity, *RigidForce);
                                    else // Was previously already colliding
                                        BroadcastGlobalEvent<UnitTriggerStayEvent>(Entity);

                                    Boundingbox->m_Collided = /*BB->m_Collided = */true;
                            }
                            else
                                if (Boundingbox->m_Collided)
                                    BroadcastGlobalEvent<UnitTriggerExitEvent>(Entity, *RigidForce);
                            Boundingbox->m_Collided = /*BB->m_Collided = */false;
                        }
                    }
                    else
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
                            // Remove once layering component integration is resolved
                                // If first instance of collision (may be an issue with gravity)
                                if (!Boundingbox->m_Collided)
                                    BroadcastGlobalEvent<UnitTriggerEvent>(Entity, *RigidForce);
                                else // Was previously already colliding
                                    BroadcastGlobalEvent<UnitTriggerStayEvent>(Entity);

                            Boundingbox->m_Collided = /*BB->m_Collided = */true;
                        }
                        else
                        {
                            if (Boundingbox->m_Collided)
                                BroadcastGlobalEvent<UnitTriggerExitEvent>(Entity, *RigidForce);

                            Boundingbox->m_Collided = /*BB->m_Collided = */false;
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
                return false;
            });
    }
};