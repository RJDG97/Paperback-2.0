#pragma once

#include "Physics/geometry.h"
#include "Systems/DebugSystem.h"

struct bullet_logic_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "bullet_logic_system"
    };
    // Entity to test component -> jp side
    void operator()( /*paperback::component::entity& Entity,*/ transform& Transform, /*rigidbody& rb,*/ paperback::component::entity& Entity, BoundingBox* boundingbox, Sphere* sphere) noexcept
    {
        if (Entity.IsZombie()) return;

        //Timer.m_Timer -= m_Coordinator.DeltaTime();
        //if ( Timer.m_Timer <= 0.0f )
        //{
        //    m_Coordinator.DeleteEntity( Entity );
        //    return;
        //}

        // Check collisions
        tools::query Query;
        Query.m_Must.AddFromComponents<transform>();

        paperback::Vector3f tf = { Transform.m_Position.m_X, Transform.m_Position.m_Y, Transform.m_Position.m_Z };
        paperback::Vector3f xf;

        if (sphere)
            sphere->setCenter(tf);

        ForEach(Search(Query), [&](paperback::component::entity& Dynamic_Entity, transform& xform, BoundingBox* bb, Sphere* ball) noexcept -> bool
            {
                assert(Entity.IsZombie() == false);

                // Do not check against self
                if ((&Entity == &Dynamic_Entity) || (Dynamic_Entity.IsZombie()) /* || (Bullet.m_Owner.m_GlobalIndex == Dynamic_Entity.m_GlobalIndex)*/) return false;

                xf.x = xform.m_Position.m_X;
                xf.y = xform.m_Position.m_Y;
                xf.z = xform.m_Position.m_Z;

                if (boundingbox && bb)
                {
                    // collision detection part
                    if (AabbAabb(tf + boundingbox->MinMax[0], tf + boundingbox->MinMax[1], xf + bb->MinMax[0], xf + bb->MinMax[1]))
                    {
                        boundingbox->m_Collided = bb->m_Collided = true;
                    }
                    else
                        boundingbox->m_Collided = bb->m_Collided = false;
                }
                
                if (sphere && ball)
                {
                    if (SphereSphere(sphere->getCenter(), sphere->getRadius(), ball->getCenter(), ball->getRadius()))
                    {
                        sphere->m_Collided = ball->m_Collided = true;
                    }
                    else
                        sphere->m_Collided = ball->m_Collided = false;
                }
                
                constexpr auto min_distance_v = 4;
                if ((Transform.m_Position - xform.m_Position).getLengthSquared() < min_distance_v * min_distance_v)
                {
                    //m_Coordinator.DeleteEntity(Entity);
                    //m_Coordinator.DeleteEntity(Dynamic_Entity);
                    return true;
                }
                return false;
            });
    }
};