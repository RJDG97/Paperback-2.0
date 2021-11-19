#pragma once
#include "Math/Vector3f.h"

struct physics_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "physics_system"
    };

    int DebugTest = 0;

    using query = std::tuple
    <
        paperback::query::none_of<prefab>
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
            });
    }

    // map check collision out of bounds check
    void operator()(paperback::component::entity& Entity, transform& Transform, rigidbody* RigidBody, rigidforce* RigidForce, mass* Mass) noexcept
    {
        if (RigidForce != nullptr)
        {
            // minimum value threshold
            RigidForce->m_Forces.CutoffValue(RigidForce->m_minthreshold);
            RigidForce->m_Momentum.CutoffValue(RigidForce->m_minthreshold);

            // momentum && accel
            if (!RigidForce->m_Momentum.IsZero() && !RigidForce->m_Forces.IsZero())
            {
                // friction
                RigidForce->m_Momentum.DecrementValue(
                    RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());

                RigidForce->m_Momentum += (RigidForce->m_Forces * m_Coordinator.DeltaTime());

                RigidForce->m_Forces.DecrementValue(
                    RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());
            }
            // momentum
            else if (RigidForce->m_Forces.IsZero())
            {
                // friction
                RigidForce->m_Momentum.DecrementValue(
                    RigidForce->m_dynamicFriction * m_Coordinator.DeltaTime());
            }

            // accumulate result
            if (RigidBody && Mass)
            {
                RigidBody->m_Accel = ConvertToAccel(Mass->m_Mass, RigidForce->m_Forces);
                RigidBody->m_Velocity = ConvertToVelocity(Mass->m_Mass, RigidForce->m_Momentum);
                Transform.m_Position += RigidBody->m_Velocity * m_Coordinator.DeltaTime();
            }
        }
    }
};