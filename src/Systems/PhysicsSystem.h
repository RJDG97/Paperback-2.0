#pragma once
// physics************************************************************ boundary system aka map
#include "Math/MathUtils.h"

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

    //helper function to ensure that momentum is -ve or +ve depending on current momentum
    paperback::Vector3f SetMaxMoment(paperback::Vector3f& currmoment, paperback::Vector3f& max)
    {

        return {
            (abs(currmoment.x) > 0.01f) ? (currmoment.x < 0.0f) ? max.x : -max.x : 0.0f,
            (abs(currmoment.y) > 0.01f) ? (currmoment.y < 0.0f) ? max.y : -max.y : 0.0f,
            (abs(currmoment.z) > 0.01f) ? (currmoment.z < 0.0f) ? max.z : -max.z : 0.0f
        };
        //  if ( +ve > 0.1f )   else if  ( -ve < 0.f )    else 0.f
    }

    //test helper function to apply forces on all entities with rigidforce components
    void ApplyForceAll(paperback::Vector3f vec)
    {
        // ideally u don't keep adding momentum, obj will be team rocket blasting off
        //    momentum speed bleed off will be slower than the adding of momentum
        tools::query Query;
        Query.m_Must.AddFromComponents<transform, rigidbody, rigidforce>();
        
        ForEach(Search(Query), [&](paperback::component::entity& Entity, transform& xform, rigidbody& rb, rigidforce& rf) noexcept
            {
                assert(Entity.IsZombie() == false);
                
                rf.AddMomentum(vec);
                rf.m_MagMoment = 1.0f; // this is magnitudeSq
            });
    }

    //test helper function to apply forces on all entities with rigidforce components
    void ApplyAccelAll(paperback::Vector3f vec)
    {

        tools::query Query;
        Query.m_Must.AddFromComponents<transform, rigidbody, rigidforce>();
        
        ForEach(Search(Query), [&](paperback::component::entity& Entity, transform& xform, rigidbody& rb, rigidforce& rf) noexcept
            {
                assert(Entity.IsZombie() == false);
                
                rf.AddForce(vec);
                rf.m_MagForce = 1.0f; // this is magnitudeSq
                rf.m_isAccel = true;
            });
    }

    //test helper function to decelerate on all entities with rigidforce components
    void NotAccelerating()
    {

        tools::query Query;
        Query.m_Must.AddFromComponents<transform, rigidbody, rigidforce>();

        ForEach(Search(Query), [&](paperback::component::entity& Entity, transform& xform, rigidbody& rb, rigidforce& rf) noexcept
            {
                assert(Entity.IsZombie() == false);
                rf.m_isAccel = false;
            });
    }

    // map check collision out of bounds check
    void operator()(paperback::component::entity& Entity, transform& Transform, rigidbody* RigidBody, rigidforce* RigidForce) noexcept
    {
        // should i use a check to limit deltaTime() value in case of lag?? time = 10.f

        // physics before bullet_logic******
        // if force/mass exist
        if (RigidForce != nullptr)
        {
            // ideally placed at the back after calculations, comment once finished, don't delete (debug)
            // Caps max_accel
            RigidForce->m_Forces.LockingValue(RigidForce->m_MaxForce);
            // Caps max_velocity
            RigidForce->m_Momentum.LockingValue(RigidForce->m_MaxMoment);

            // accelerating, this part does not decay acceleration / momentum
            if (RigidForce->m_isAccel)
            {
                // update momentum
                RigidForce->m_Momentum += (RigidForce->m_Forces * m_Coordinator.DeltaTime());
            }
            // decelerating
            else
            {
                // cutoff for min acceleration, cutoff set to 0.f
                RigidForce->m_Forces.CutoffValue(RigidForce->m_minthreshold);
                // cutoff for min velocity, cutoff set to 0.f
                RigidForce->m_Momentum.CutoffValue(RigidForce->m_minthreshold);

                // no velocity and acceleration
                if (RigidForce->m_Momentum.IsZero() && RigidForce->m_Forces.IsZero())
                {
                    // do nothing since velocity = 0
                }
                // only process velocity
                else if (RigidForce->m_Forces.IsZero())
                {
                    // this part is to bleed out any remaining velocity after Accel = 0.f
                    //RigidForce->m_MagMoment = RigidForce->m_Momentum.MagnitudeSq(); this doesn't work

                    // friction in action based on time, updates non-zero values
                    RigidForce->m_Momentum.DecrementValue(
                        RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());

                    //// treated as 0 momentum when threshold reached
                    //if (RigidForce->m_Momentum.IsZero())
                    //{
                    //    ;
                    //}
                    //// otherwise, update momentum
                    //else
                    //{
                    //    ////// this part is to bleed out any remaining velocity after Accel = 0.f
                    //    //float Mproportion = RigidForce->m_MagMoment / (RigidForce->m_MaxMomentSq);
                    //    // paperback::Vector3f max = SetMaxMoment(RigidForce->m_Momentum, RigidForce->m_MaxMoment);
                    //
                    //    // friction in action based on time, updates non-zero values
                    //    RigidForce->m_Momentum.DecrementValue(
                    //        RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());
                    //
                    //    //force momentum to reduce to prevent unstopping movement
                    //    //RigidForce->m_Momentum *= 0.98f;
                    //}
                }
                // process acceleration and velocity
                else
                {
                    // this part here is optional
                    // friction in action based on time, updates non-zero values
                    RigidForce->m_Momentum.DecrementValue(
                        RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());

                    // add force to momentum based on time
                    RigidForce->m_Momentum += (RigidForce->m_Forces * m_Coordinator.DeltaTime());

                    // friction in action based on time, updates non-zero values
                    RigidForce->m_Forces.DecrementValue(
                        RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());
                }

                
                //****************************************
                
                //************************
                //lock momentum within maximum allowed 
                //if (RigidForce->m_MagMoment < RigidForce->m_MaxMoment.MagnitudeSq())
                //{
                //    ////// this part is to bleed out any remaining velocity after Accel = 0.f
                //    // current momentum / maximum momentum / 2
                //    float Mproportion = RigidForce->m_MagMoment / (RigidForce->m_MaxMomentSq);
                //    // update momentum
                //    //account for -ve and +ve increment
                //    paperback::Vector3f max = SetMaxMoment(RigidForce->m_Momentum, RigidForce->m_MaxMoment);
                //
                //    RigidForce->m_Momentum -= (Mproportion * max * m_Coordinator.DeltaTime());
                //}
                //
                ////force momentum to reduce to prevent unstopping movement
                //RigidForce->m_Momentum *= 0.98f;
                //****************************************
            }


            // assign calculated values into rigidBody
            RigidBody->m_Accel = RigidForce->ConvertToAccel().ConvertMathVecToXcoreVec();
            RigidBody->m_Velocity = RigidForce->ConvertToVelocity().ConvertMathVecToXcoreVec();
        }
        else
        {
            // remember that rigid body without RIGID FORCE does not have a velocity or accel dampener
        }

        Transform.m_Position += RigidBody->m_Velocity * m_Coordinator.DeltaTime();

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