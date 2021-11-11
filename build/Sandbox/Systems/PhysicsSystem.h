#pragma once
#include "Math/Vector3f.h"

struct physics_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "physics_system"
    };

    int DebugTest = 0;

    using query = std::tuple<
        paperback::query::one_of<>
    >;

    // Helper function to ensure that momentum is -ve or +ve depending on current momentum
    paperback::Vector3f SetMaxMoment(paperback::Vector3f& CurrMoment, paperback::Vector3f& Max)
    {

        return {
            (abs(CurrMoment.x) > 0.01f) ? (CurrMoment.x < 0.0f) ? Max.x : -Max.x : 0.0f,
            (abs(CurrMoment.y) > 0.01f) ? (CurrMoment.y < 0.0f) ? Max.y : -Max.y : 0.0f,
            (abs(CurrMoment.z) > 0.01f) ? (CurrMoment.z < 0.0f) ? Max.z : -Max.z : 0.0f
        };
    }

    void AddForce(paperback::Vector3f& Forces, const paperback::Vector3f& Force)
    {

        Forces += Force;
    }

    void AddMomentum(paperback::Vector3f& Momentum, const paperback::Vector3f& Moment)
    {

        Momentum += Moment;
    }

    void ResetMomentum(paperback::Vector3f& Momentum)
    {

        Momentum = paperback::Vector3f{};
    }

    paperback::Vector3f ConvertToAccel(const float Mass, const paperback::Vector3f& Forces)
    {
        return (Mass > 0) ? Forces / Mass : paperback::Vector3f{ 0.0f, 0.0f, 0.0f };
    }

    paperback::Vector3f ConvertToVelocity(const float Mass, const paperback::Vector3f& Momentum)
    {
        return  (Mass > 0) ? Momentum / Mass : paperback::Vector3f{ 0.0f, 0.0f, 0.0f };
    }

    //test helper function to apply forces on all entities with rigidforce components
    void ApplyForceAll(paperback::Vector3f Vec)
    {
        tools::query Query;
        Query.m_Must.AddFromComponents<transform, rigidbody, rigidforce>();

        ForEach(Search(Query), [&](paperback::component::entity& Entity, transform& Xform, rigidbody& RB, rigidforce& RF) noexcept
            {
                assert(Entity.IsZombie() == false);

                if (!RF.m_isStatic)
                {
                    AddMomentum(RF.m_Momentum, Vec);
                    RF.m_MagMoment = 1.0f;
                }
            });
    }

    //test helper function to apply forces on all entities with rigidforce components
    void ApplyAccelAll(paperback::Vector3f Vec)
    {

        tools::query Query;
        Query.m_Must.AddFromComponents<transform, rigidbody, rigidforce>();

        ForEach(Search(Query), [&](paperback::component::entity& Entity, transform& Xform, rigidbody& RB, rigidforce& RF) noexcept
            {
                assert(Entity.IsZombie() == false);

                AddForce(RF.m_Forces, Vec);
                RF.m_MagForce = 1.0f; // this is magnitudeSq
                //RF.m_isAccel = true;
            });
    }

    //test helper function to decelerate on all entities with rigidforce components
    void NotAccelerating()
    {

        tools::query Query;
        Query.m_Must.AddFromComponents<transform, rigidbody, rigidforce>();

        ForEach(Search(Query), [&](paperback::component::entity& Entity, transform& Xform, rigidbody& RB, rigidforce& RF) noexcept
            {
                assert(Entity.IsZombie() == false);
                RF.m_isAccel = false;
            });
    }

    // to change: physics objects need rigidbody, optional?(rigidforce)

    // map check collision out of bounds check
    void operator()(paperback::component::entity& Entity, transform& Transform, rigidbody* RigidBody, rigidforce* RigidForce, WaypointUserv1* wu) noexcept
    {
        // RigidForce -> RigidBody -> transform
        if (RigidForce != nullptr)
        {
            // waypoint users have rigidForce && zero decay of movement (rigidForce have inherent decay)
            if (wu)
            {
                // in case it goes minimum crazy
                RigidForce->m_Forces.CutoffValue(RigidForce->m_minthreshold);
                RigidForce->m_Momentum.CutoffValue(RigidForce->m_minthreshold);

                // no decrement of movement -> makes movement predictable
            }

            // non-waypoint users
            else if (RigidForce->m_isAccel)
            {
                RigidForce->m_Momentum += (RigidForce->m_Forces * m_Coordinator.DeltaTime());
            }
            else
            {
                // minimum value threshold
                RigidForce->m_Forces.CutoffValue(RigidForce->m_minthreshold);
                RigidForce->m_Momentum.CutoffValue(RigidForce->m_minthreshold);

                // momentum && accel
                if (!RigidForce->m_Momentum.IsZero() && !RigidForce->m_Forces.IsZero())
                {
                    RigidForce->m_Momentum.DecrementValue(
                        RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());

                    RigidForce->m_Momentum += (RigidForce->m_Forces * m_Coordinator.DeltaTime());

                    RigidForce->m_Forces.DecrementValue(
                        RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());
                }
                // momentum
                else if (RigidForce->m_Forces.IsZero())
                {
                    RigidForce->m_Momentum.DecrementValue(
                        RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());
                }
            }

            // assign values and new position, rb delete
            if (RigidBody)
            {
                RigidBody->m_Accel = ConvertToAccel(RigidForce->m_Mass, RigidForce->m_Forces);
                RigidBody->m_Velocity = ConvertToVelocity(RigidForce->m_Mass, RigidForce->m_Momentum);

                // to be changed
                if (wu && wu->isAttacking)
                {
                    // stay still and attacks
                }
                else
                {
                    Transform.m_Position += RigidBody->m_Velocity * m_Coordinator.DeltaTime();
                }
            }
        }

        

        /*
            Remove if unnecessary - Add Remove Component Reference
            1st Tuple = Add
            2nd Tuple = Remove
        */
        {
            //// Remove Bullet Component Test
            //if ( !BulletTest )
            //    TRACE_PRINT( "Component removed successfully" );

            //if ( BulletTest && Entity )
            //{
            //    TRACE_PRINT( "Entity Global Index: " + std::to_string(Entity->m_GlobalIndex) );

            //    auto UpdatedEntityComponent = m_Coordinator.AddOrRemoveComponents< std::tuple<>,                          // Tuple of Components to Add
            //                                                                       std::tuple<bullet> >( *Entity );       // Tuple of Components to Remove

            //    INFO_PRINT( "Removing bullet component from entity in Physics System" );
            //    return;
            //}


            //// Add Bullet Component Test
            //if ( BulletTest )
            //    TRACE_PRINT( "Component added successfully - ID: " + std::to_string( Entity->m_GlobalIndex ) );

            //if ( !BulletTest && Entity )
            //{
            //    TRACE_PRINT( "Entity Global Index: " + std::to_string(Entity->m_GlobalIndex) );

            //    auto UpdatedEntityComponent = m_Coordinator.AddOrRemoveComponents< std::tuple<bullet> >( *Entity );

            //    INFO_PRINT( "Adding bullet component to entity in Physics System" );
            //    return;
            //}
        }

        // X-Out-Of-Bounds
        /*if (Transform.m_Position.m_X < 0.0f)
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
    }
};