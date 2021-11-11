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

    void operator()( paperback::component::entity& Entity, transform& Transform, /*rigidbody& rb,*/rigidforce* RigidForce,  boundingbox * Boundingbox, sphere* Sphere, Collidable* col1,
        WaypointUserv1* wu1, Waypointv1* wp1) noexcept
    {
        if ( Entity.IsZombie() ) return;

        // Initialize Query
        tools::query Query;
        Query.m_Must.AddFromComponents < transform >();
        Query.m_OneOf.AddFromComponents< boundingbox, sphere >();

        paperback::Vector3f tf = { Transform.m_Position.x + Transform.m_Offset.x, Transform.m_Position.y + Transform.m_Offset.y, Transform.m_Position.z + Transform.m_Offset.z };
        paperback::Vector3f xf;

        ForEach( Search( Query ), [&](paperback::component::entity& Dynamic_Entity, transform& Xform, rigidforce* RF, boundingbox* BB, sphere* Ball, Collidable* col2,
            WaypointUserv1* wu2, Waypointv1* wp2) noexcept -> bool
            {
                assert(Entity.IsZombie() == false);

                // Do not check against self
                if ( (&Entity == &Dynamic_Entity) || (Dynamic_Entity.IsZombie()) ) return false;

                xf.x = Xform.m_Position.x + Xform.m_Offset.x;
                xf.y = Xform.m_Position.y + Xform.m_Offset.y;
                xf.z = Xform.m_Position.z + Xform.m_Offset.z;

                // if Shape exist
                if (Boundingbox && BB)
                {
                    // Collision Detection - added to parameters
                    if (AabbAabb(tf + Boundingbox->Min, tf + Boundingbox->Max, xf + BB->Min, xf + BB->Max))
                    {
                        // swap to before aabbaabb
                        if (col1->m_CollidableLayers.Has(col2->m_CollisionLayer ) )
                        {
                            // Collision Response because Entity is marked as collidable against ForEach_Entity
                            //if (RigidForce && RF /* && !Boundingbox->m_Collided*/)
                            //{
                            //    bool checker = CheapaabbDynamic(
                            //        Boundingbox,
                            //        RigidForce,
                            //        Transform,
                            //        BB,
                            //        RF,
                            //        Xform);
                            //}
                            
                            // if both waypoint users collides
                            if (wu1 && wu2)
                            {
                                // stop users to commence atk
                                wu1->isAttacking = wu2->isAttacking = true;
                            }

                            // unit collide with waypoint
                            else if (wu1 && wp2)
                            {
                                if(wp2->isBase)
                                    wu1->isAttacking = true;
                                else
                                {
                                    wu1->m_destination = wp2->m_destination[wu1->m_player];

                                    // be careful with this!! set speed if there is one
                                    if (RigidForce && wp2->b_enableSpeed)
                                    {
                                        // first var is the user
                                        RigidForce->m_Momentum = wp2->m_setspeed[wu1->m_player] * RigidForce->m_Mass;
                                    }
                                }
                            }
                            
                            else if (wu2 && wp1)
                            {
                                if (wp1->isBase)
                                    wu2->isAttacking = true;
                                else
                                {
                                    wu2->m_destination = wp1->m_destination[wu2->m_player];

                                    // be careful with this!! set speed if there is one
                                    if (RigidForce && wp1->b_enableSpeed)
                                    {
                                        // first var is the user
                                        RigidForce->m_Momentum = wp1->m_setspeed[wu2->m_player] * RigidForce->m_Mass;
                                    }
                                }
                            }
                        }
                        Boundingbox->m_Collided = BB->m_Collided = true;
                    }
                    
                    else
                    {
                        // color doesnot change cause it's overwritten when checked with other objs
                        Boundingbox->m_Collided = BB->m_Collided = false;
                        
                        // so that it doesn't check with other random units and waypoints and set false
                        if (col1->m_CollidableLayers.Has(col2->m_CollisionLayer))
                        {
                            // because of the way the iteration works(architecture), player 2 won't respond
                            // if on the same collision layer, it checks with another obj to set 
                            // isAttacking = false,  therefore the bug with player 2 and not player 1
                            
                            // waypoint users, if wu1 or wu2 is DELETED, no way to change this boolean. 
                            // set it manually to resume troop movement ***!!!!!
                            //if (wu1 && wu2)
                            //{
                            //    wu1->isAttacking = wu2->isAttacking = false;
                            //}
                            //// it case there are multiple bases
                            //if (wu1 && wp2 && wp->)
                            //{
                            //    wu1->isAttacking = false;
                            //}
                            //if (wu2 && wp1)
                            //{
                            //    wu2->isAttacking = false;
                            //}
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

            // Note:
            // only bug left (2nd player does not collide and debugDraw to red against 1st player)
    }
};