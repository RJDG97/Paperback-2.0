#pragma once
#include "Math/Vector3f.h"

const float GRAVITY = -15.8f; //-9.8 is default, use higher value for release due to lower decrement for some reason

struct physics_system : paperback::system::pausable_instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "physics_system"
    };

    tools::query Query;

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

    void ApplyVelocityCap( paperback::Vector3f& Velocity ) noexcept
    {
        Velocity.x = Velocity.x < 0 
                     ? Velocity.x < -paperback::settings::velocity_axis_cap_v 
                       ? -paperback::settings::velocity_axis_cap_v
                       : Velocity.x
                     : Velocity.x > paperback::settings::velocity_axis_cap_v 
                       ? paperback::settings::velocity_axis_cap_v
                       : Velocity.x;
        Velocity.y = Velocity.y < 0 
                     ? Velocity.y < -paperback::settings::velocity_axis_cap_v 
                       ? -paperback::settings::velocity_axis_cap_v
                       : Velocity.y
                     : Velocity.y > paperback::settings::velocity_axis_cap_v 
                       ? paperback::settings::velocity_axis_cap_v
                       : Velocity.y;
        Velocity.z = Velocity.z < 0 
                     ? Velocity.z < -paperback::settings::velocity_axis_cap_v 
                       ? -paperback::settings::velocity_axis_cap_v
                       : Velocity.z
                     : Velocity.z > paperback::settings::velocity_axis_cap_v 
                       ? paperback::settings::velocity_axis_cap_v
                       : Velocity.z;
    }

    void ApplyMomentumCap( paperback::Vector3f& Momentum ) noexcept
    {
        Momentum.x = Momentum.x < 0 
                     ? Momentum.x < -paperback::settings::momentum_axis_cap_v 
                       ? -paperback::settings::momentum_axis_cap_v
                       : Momentum.x
                     : Momentum.x > paperback::settings::momentum_axis_cap_v 
                       ? paperback::settings::momentum_axis_cap_v
                       : Momentum.x;
        Momentum.y = Momentum.y < 0 
                     ? Momentum.y < -paperback::settings::momentum_axis_cap_v 
                       ? -paperback::settings::momentum_axis_cap_v
                       : Momentum.y
                     : Momentum.y > paperback::settings::momentum_axis_cap_v 
                       ? paperback::settings::momentum_axis_cap_v
                       : Momentum.y;
        Momentum.z = Momentum.z < 0 
                     ? Momentum.z < -paperback::settings::momentum_axis_cap_v 
                       ? -paperback::settings::momentum_axis_cap_v
                       : Momentum.z
                     : Momentum.z > paperback::settings::momentum_axis_cap_v 
                       ? paperback::settings::momentum_axis_cap_v
                       : Momentum.z;
    }


    PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		Query.m_Must.AddFromComponents<transform, entity, rigidforce>();
		Query.m_OneOf.AddFromComponents<boundingbox, mass, rigidbody, rotation, player_controller>();
        Query.m_OneOf.AddFromComponents<name, child, offset>();
		Query.m_NoneOf.AddFromComponents<prefab>();
	}

    PPB_INLINE
	void OnStateChange( void ) noexcept
	{
	}

    PPB_FORCEINLINE
	void Update( void ) noexcept
	{
		ForEach( Search( Query ), [&]( entity& Entity, transform& Transform, rigidforce& RigidForce, rigidbody* RigidBody, rotation* Rot, mass* Mass, boundingbox* Box, name* Name, child* Child, offset* Offset, player_controller* Controller ) noexcept
		{
            //// Apply Gravity If Non-Static
            if (Mass && Mass->m_Mass && RigidForce.m_GravityAffected)
            {

                if (RigidForce.m_GravityActive)
                {
                    RigidForce.m_Forces.y += GRAVITY * Mass->m_Mass * DeltaTime();
                }
                else
                {
                    RigidForce.m_GravityActive = true;
                    if ( Controller ) Controller->m_OnGround = true;
                }
            }

            // minimum value threshold
            RigidForce.m_Forces.CutoffValue(RigidForce.m_minthreshold);
            RigidForce.m_Momentum.CutoffValue(RigidForce.m_minthreshold);

            // Cap Momentum Values
            ApplyMomentumCap( RigidForce.m_Momentum );

            // momentum && accel only
            if (!RigidForce.m_Momentum.IsZero() && !RigidForce.m_Forces.IsZero())
            {
                // friction in action, reduces momentum
                RigidForce.m_Momentum.DecrementValue(
                    RigidForce.m_dynamicFriction * m_Coordinator.DeltaTime());

                // accumulate momentum
                RigidForce.m_Momentum += ( RigidForce.m_Forces * m_Coordinator.DeltaTime() );

                // friction in action, reduces acceleration
                RigidForce.m_Forces.DecrementValue(
                    RigidForce.m_dynamicFriction * m_Coordinator.DeltaTime());
            }
            // momentum only
            else if (RigidForce.m_Forces.IsZero())
            {
                // friction in action, reduces momentum
                RigidForce.m_Momentum.DecrementValue(
                    RigidForce.m_dynamicFriction * m_Coordinator.DeltaTime());
            }
            else if (RigidForce.m_Momentum.IsZero())
            {

                // accumulate momentum
                RigidForce.m_Momentum += (RigidForce.m_Forces * m_Coordinator.DeltaTime());

                // friction in action, reduces acceleration
                RigidForce.m_Forces.DecrementValue(
                    RigidForce.m_dynamicFriction * m_Coordinator.DeltaTime());
            }

            // accumulate result into rigidbody and update position
            if ( RigidBody && Mass )
            {
                if (Child && Offset)
                {
                    // Save Old Position For Grid Update
                    auto OldPosition = Offset->m_PosOffset;

                    RigidBody->m_Accel = ConvertToAccel(Mass->m_Mass, RigidForce.m_Forces);
                    RigidBody->m_Velocity = ConvertToVelocity(Mass->m_Mass, RigidForce.m_Momentum);

                    // Apply Velocity Cap
                    ApplyVelocityCap( RigidBody->m_Velocity );

                    Offset->m_PosOffset += RigidBody->m_Velocity * m_Coordinator.DeltaTime();// +0.5f * (RigidBody->m_Accel * m_Coordinator.DeltaTime() * m_Coordinator.DeltaTime());

                    // Update Hash Grid - On Position Update
                    if (Box)
                    {
                        m_Coordinator.UpdateUnit(Entity.m_GlobalIndex
                            , OldPosition
                            , Offset->m_PosOffset
                            , Box->Min
                            , Box->Max);
                    }
                }

                else
                {
                    // Save Old Position For Grid Update
                    auto OldPosition = Transform.m_Position;

                    RigidBody->m_Accel = ConvertToAccel(Mass->m_Mass, RigidForce.m_Forces);
                    RigidBody->m_Velocity = ConvertToVelocity(Mass->m_Mass, RigidForce.m_Momentum);

                    // Apply Velocity Cap
                    ApplyVelocityCap( RigidBody->m_Velocity );

                    Transform.m_Position += RigidBody->m_Velocity * m_Coordinator.DeltaTime();// +0.5f * (RigidBody->m_Accel * m_Coordinator.DeltaTime() * m_Coordinator.DeltaTime());

                    // Update Hash Grid - On Position Update
                    if ( Box )
                    {  
                        m_Coordinator.UpdateUnit( Entity.m_GlobalIndex
                                                , OldPosition
                                                , Transform.m_Position
                                                , Box->Min
                                                , Box->Max );
                    }
                }
                
            }
            // Update Rotation
            if ( RigidBody && Rot && RigidBody->m_Velocity.MagnitudeSq() > 0.01f )
            {
                auto Debug = m_Coordinator.FindSystem<debug_system>();
                if ( Debug )
                    Rot->m_Value.y = Debug->DirtyRotationAnglesFromDirectionalVec( RigidBody->m_Velocity ).y;
            }
        });
	}
};