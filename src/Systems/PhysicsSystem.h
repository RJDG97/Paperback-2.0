#pragma once
#include "Components/Transform.h"
#include "Components/Rigidbody.h"
#include "Math/Math_includes.h"
#include "Systems/DebugSystem.h"
#include "Physics/PhysicsStructs.h"

// physics************************************************************ boundary system aka map
struct physics_system : paperback::system::instance
{

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "physics_system"
    };

    
    //helper function
    //adds force to a specific entity with player tag
    void AddForceToPlayer(float dir_x, float dir_y, float dir_z, float magnitude, std::string tag = "", float lifetime = 1.0f)
    {

        tools::query Query;
        Query.m_Must.AddFromComponents<transform, rigidbody, player>();

        ForEach(Search(Query), [&](paperback::component::entity& Entity, transform& xform, rigidbody& rb, player& player) noexcept
            {
                assert(Entity.IsZombie() == false);

                rb.m_ForceHandler.AddForce(dir_x, dir_y, dir_z, magnitude, tag, lifetime);


            });

    }

    void OnFrameStart(void) noexcept 
    {
        
        GetSystem<debug_system>().BeginTime(1);
    }

    void OnFrameEnd(void) noexcept 
    {
        
        GetSystem<debug_system>().EndTime(1);
    }


    // map check collision out of bounds check
    void operator()(paperback::component::entity& Entity, transform& Transform, rigidbody& RigidBody ) noexcept
    {

        RigidBody.m_ForceHandler.GetForces(DeltaTime());



        /*
        // X-Out-Of-Bounds
        if  (Transform.m_Position.m_X < 0.0f)
        {
            Transform.m_Position.m_X = 0.0f;
            RigidBody.m_Velocity.m_X = -RigidBody.m_Velocity.m_X;
        }
        else if (Transform.m_Position.m_X >= m_Engine.m_Width)
        {
            Transform.m_Position.m_X = m_Engine.m_Width;
            RigidBody.m_Velocity.m_X = -RigidBody.m_Velocity.m_X;
        }

        // Y-Out-Of-Bounds
        if (Transform.m_Position.m_Y < 0.0f)
        {
            Transform.m_Position.m_Y = 0.0f;
            RigidBody.m_Velocity.m_Y = -RigidBody.m_Velocity.m_Y;
        }
        else if (Transform.m_Position.m_Y >= m_Engine.m_Height)
        {
            Transform.m_Position.m_Y = m_Engine.m_Height;
            RigidBody.m_Velocity.m_Y = -RigidBody.m_Velocity.m_Y;
        }*/

        RigidBody.m_Velocity = RigidBody.m_ForceHandler.m_TotalForces;

        Transform.m_Position += RigidBody.m_Velocity * m_Coordinator.DeltaTime();

        xcore::vector3 new_pos = Transform.m_Position + Transform.m_Offset;

        Transform.fakeSphere.setCenter({ new_pos.m_X, new_pos.m_Y, new_pos.m_Z });
    }
};