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
    void operator()( /*paperback::component::entity& Entity,*/ transform& Transform, /*rigidbody& rb,*/ paperback::component::entity& Entity, boundingbox* Boundingbox, sphere* Sphere) noexcept
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
        Query.m_OneOf.AddFromComponents<boundingbox, sphere>();

        paperback::Vector3f tf = { Transform.m_Position.x + Transform.m_Offset.x, Transform.m_Position.y + Transform.m_Offset.y, Transform.m_Position.z + Transform.m_Offset.z };
        paperback::Vector3f xf;

        ForEach(Search(Query), [&](paperback::component::entity& Dynamic_Entity, transform& Xform, boundingbox* BB, sphere* Ball) noexcept -> bool
            {
                assert(Entity.IsZombie() == false);

                // Do not check against self
                if ((&Entity == &Dynamic_Entity) || (Dynamic_Entity.IsZombie()) /* || (Bullet.m_Owner.m_GlobalIndex == Dynamic_Entity.m_GlobalIndex)*/) return false;

                xf.x = Xform.m_Position.x + Xform.m_Offset.x;
                xf.y = Xform.m_Position.y + Xform.m_Offset.y;
                xf.z = Xform.m_Position.z + Xform.m_Offset.z;

                // collision detection part
                if (Boundingbox && BB && AabbAabb(tf + Boundingbox->Min, tf + Boundingbox->Max, xf + BB->Min, xf + BB->Max))
                {
                    Boundingbox->m_Collided = BB->m_Collided = true;
                }

                if (Sphere && Ball && SphereSphere(tf, Sphere->m_fRadius, xf, Ball->m_fRadius))
                {
                    Sphere->m_Collided = Ball->m_Collided = true;
                }

                constexpr auto min_distance_v = 4;
                if ((Transform.m_Position - Xform.m_Position).MagnitudeFast() < min_distance_v * min_distance_v)
                {
                    //m_Coordinator.DeleteEntity(Entity);
                    //m_Coordinator.DeleteEntity(Dynamic_Entity);
                    return true;
                }
                return false;
            });
    }
};