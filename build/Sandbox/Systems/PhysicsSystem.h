#pragma once
#include "Math/Vector3f.h"

const float GRAVITY = -15.8f; //-9.8 is default, use higher value for release due to lower decrement for some reason

struct physics_system : paperback::system::pausable_instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "physics_system"
    };

    // Events
    struct Event_OnStatic : paperback::event::instance< entity& > {};
    struct Event_OnFalling : paperback::event::instance< entity& > {};

    // Data Member
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

    bool MovementEnabled( void ) noexcept
    {
        return ( m_Coordinator.IsKeyPressDown( GLFW_KEY_W ) ||
                 m_Coordinator.IsKeyPressDown( GLFW_KEY_A ) ||
                 m_Coordinator.IsKeyPressDown( GLFW_KEY_S ) ||
                 m_Coordinator.IsKeyPressDown( GLFW_KEY_D ) ||
                 m_Coordinator.IsGamepadButtonPressDown( GLFW_GAMEPAD_BUTTON_LEFT_THUMB ) );
    }

    paperback::Vector3f ConvertToAccel(const float Mass, const paperback::Vector3f& Forces)
    {
        return (Mass > 0) ? Forces / Mass : paperback::Vector3f{ 0.0f, 0.0f, 0.0f };
    }

    paperback::Vector3f ConvertToVelocity(const float Mass, const paperback::Vector3f& Momentum)
    {
        return  (Mass > 0) ? Momentum / Mass : paperback::Vector3f{ 0.0f, 0.0f, 0.0f };
    }

    void ApplyAxisCap( float& Val, const float Cap ) noexcept
    {
        Val = Val < 0 
              ? Val < -Cap 
                ? -Cap
                : Val
              : Val > Cap 
                ? Cap
                : Val;
    }

    void ApplyVelocityCap( paperback::Vector3f& Velocity ) noexcept
    {
        ApplyAxisCap( Velocity.x, paperback::settings::velocity_axis_cap_v );
        ApplyAxisCap( Velocity.y, paperback::settings::velocity_axis_cap_v );
        ApplyAxisCap( Velocity.z, paperback::settings::velocity_axis_cap_v );
    }

    void ApplyMomentumCap( paperback::Vector3f& Momentum ) noexcept
    {
        ApplyAxisCap( Momentum.x, paperback::settings::momentum_axis_cap_v );
        ApplyAxisCap( Momentum.y, paperback::settings::momentum_y_axis_cap_v);
        ApplyAxisCap( Momentum.z, paperback::settings::momentum_axis_cap_v );
    }


    PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		Query.m_Must.AddFromComponents<transform, entity, rigidforce>();
		Query.m_OneOf.AddFromComponents<boundingbox, mass, rigidbody, rotation, player_controller, player_interaction>();
        Query.m_OneOf.AddFromComponents<name, child, offset, particle>();
		Query.m_NoneOf.AddFromComponents<prefab>();
	}

    PPB_INLINE
	void OnStateChange( void ) noexcept
	{
	}

    PPB_FORCEINLINE
	void Update( void ) noexcept
	{
		ForEach( Search( Query ), [&]( entity& Entity, transform& Transform, rigidforce& RigidForce, rigidbody* RigidBody, rotation* Rot
                                     , mass* Mass, boundingbox* Box, name* Name, child* Child, offset* Offset, player_controller* Controller, player_interaction* Inter, particle* Particle ) noexcept
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
                    if ( Controller )
                    {
                        if ( !Controller->m_OnGround ) GetSystem<sound_system>().TriggerTaggedSound( "SFX_BlueLand" );
                        Controller->m_OnGround = true;
                    }
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

                    Offset->m_PosOffset += RigidBody->m_Velocity * m_Coordinator.DeltaTime();
                }

                else
                {
                    // Save Old Position For Grid Update
                    auto OldPosition = Transform.m_Position;

                    RigidBody->m_Accel = ConvertToAccel(Mass->m_Mass, RigidForce.m_Forces);
                    RigidBody->m_Velocity = ConvertToVelocity(Mass->m_Mass, RigidForce.m_Momentum);

                    // Apply Velocity Cap
                    ApplyVelocityCap( RigidBody->m_Velocity );

                    Transform.m_Position += RigidBody->m_Velocity * m_Coordinator.DeltaTime();
                }
                
            }
            // Update Rotation Only For Active Player
            if ( Controller && Controller->m_PlayerStatus && RigidBody && Rot && (std::fabs(RigidBody->m_Velocity.x) > 0.1f || std::fabs(RigidBody->m_Velocity.y) > 0.04f || std::fabs(RigidBody->m_Velocity.z) > 0.1f) )
            {
                auto Debug = m_Coordinator.FindSystem<debug_system>();

                // If Any Movement Key Is Pressed & Player Is Not Jumping ( Jumping resets the velocity on collision )
                if ( MovementEnabled() )
                {
                    if ( Debug && ((Inter && !Inter->m_bPushOrPull) || !Inter) )
                    {
                        // Variables
                        auto RotationSpeed = 360.0f;

                        // Assign Temp Variables
                        auto& Curr = Rot->m_Value.y;
                        float Goal = Debug->DirtyRotationAnglesFromDirectionalVec(RigidBody->m_Velocity).y;
                        // Rotate Angle to be between 0 - 360
                        Goal = Goal < 0.0f ? Goal + 360.0f : Goal;
                        // Compute Max Angle Between Curr & Goal
                        float MaxAngle = std::max(Goal, Curr) - std::min(Goal, Curr);
                        // Compute Abs Max Angle Between Curr & Goal
                        float NewMaxAngle = 360.0f - MaxAngle > MaxAngle ? 360.0f - MaxAngle : MaxAngle;
                        float CCW = 360.0f - Curr + Goal;

                        // Angle To Rotate In Frame > 5.0f
                        if ( MaxAngle > 5.0f )
                        {
                            // Rotate CCW
                            if ( Curr > Goal && CCW < NewMaxAngle )
                                Curr += RotationSpeed * DeltaTime();
                            else if ( Curr < Goal && ((Goal - Curr) < NewMaxAngle) )
                                Curr += RotationSpeed * DeltaTime();
                            // Rotate CW
                            else
                                Curr -= RotationSpeed * DeltaTime();
                        }

                        // Clamp Angles
                        Rot->m_Value.y = Rot->m_Value.y > 360.0f ? Rot->m_Value.y - 360.0f : Rot->m_Value.y;
                        Rot->m_Value.y = Rot->m_Value.y < 0.0f ? Rot->m_Value.y + 360.0f : Rot->m_Value.y;
                    }
                }
                

                if ( RigidBody->m_Velocity.y < -0.01f )
                    BroadcastGlobalEvent<Event_OnFalling>( Entity );
            }
            else
            {
                BroadcastGlobalEvent<Event_OnStatic>( Entity );
            }

            if (Box)
            {
                m_Coordinator.UpdateNode( *Box, Transform, Entity );
            }
        });
	}
};