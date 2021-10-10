#pragma once

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

                AddMomentum(RF.m_Momentum, Vec);
                RF.m_MagMoment = 1.0f;
            });
    }

    // map check collision out of bounds check
    void operator()(paperback::component::entity& Entity, transform& Transform, rigidbody* RigidBody, rigidforce* RigidForce) noexcept
    {
        if (RigidForce != nullptr)
        {
            if (RigidForce->m_isAccel)
            {
                RigidForce->m_Forces = RigidForce->m_NegForces;
            }
            else
            {
                // treated as 0 force 
                if (RigidForce->m_MagForce < RigidForce->m_threshold)
                {
                    RigidForce->m_Forces.Reset();
                    RigidForce->m_NegForces.Reset();
                    RigidForce->m_MagForce = 0.f;

                    if (RigidForce->m_MagMoment < RigidForce->m_threshold)
                    {
                        RigidForce->m_Momentum.Reset();
                        RigidForce->m_MagMoment = 0.f;
                    }
                    else
                    {
                        // this part is to bleed out any remaining velocity after Accel = 0.f
                        RigidForce->m_MagMoment = RigidForce->m_Momentum.MagnitudeSq();

                        //lock momentum within maximum allowed 
                        if (RigidForce->m_MagMoment < RigidForce->m_MaxMoment.MagnitudeSq())
                        {

                            float Mproportion = RigidForce->m_MagMoment / (RigidForce->m_MaxMomentSq * 2);
                            // update momentum
                            //account for -ve and +ve increment
                            paperback::Vector3f max = SetMaxMoment(RigidForce->m_Momentum, RigidForce->m_MaxMoment);

                            RigidForce->m_Momentum -= (Mproportion * max * m_Coordinator.DeltaTime());
                        }

                        //force momentum to reduce to prevent unstopping movement
                        RigidForce->m_Momentum *= 0.98f;
                    }
                }
                else
                {
                    // magnitudeSq
                    RigidForce->m_MagForce = RigidForce->m_Forces.MagnitudeSq();
                    // current Force / maximum Force / 2
                    float Fproportion = RigidForce->m_MagForce / (RigidForce->m_MaxForceSq * 2);
                    // forces update with time
                    RigidForce->m_Forces -= (Fproportion * RigidForce->m_MaxForce * m_Coordinator.DeltaTime());
                    RigidForce->m_NegForces = RigidForce->m_Forces;
                    // update momentum
                    RigidForce->m_Momentum -= (RigidForce->m_NegForces * m_Coordinator.DeltaTime());
                }
            }
            RigidBody->m_Accel = ConvertToAccel(RigidForce->m_Mass, RigidForce->m_Forces);
            RigidBody->m_Velocity = ConvertToVelocity(RigidForce->m_Mass, RigidForce->m_Momentum);
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