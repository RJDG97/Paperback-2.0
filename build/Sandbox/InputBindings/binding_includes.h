#pragma once

// TODO - Shift to external binding file

#ifndef PPB_INPUT_BINDING_INLINE
#define PPB_INPUT_BINDING_INLINE

/****************************************************************************************
                                ** BINDING ACTION **

  Info                |   EntityInfo
                      |
  Archetype           |   Ptr To Archetype w/ Player Controller

****************************************************************************************/

#define BEGIN_INPUT_ACTION                                                          \
    PPB_INLINE                                                                      \
    void OnInputAction( const float& Dt ) noexcept                                  \
    {                                                                               \
                                                                                    
#define END_INPUT_ACTION                                                            \
    }                                                                               \



//-----------------------------------
//         Player Scripts
//-----------------------------------

namespace paperback::input::binding
{
    PPB_INLINE
    paperback::Vector3f ConvertGLMVec3( glm::vec3 Vec ) noexcept
    {
        return paperback::Vector3f{ Vec.x, Vec.y, Vec.z };
    }

    //-----------------------------------
    //     Entity Movement Binding
    //-----------------------------------

    BEGIN_BINDING_CONSTRUCT( Keyboard_EntityMovement_Forward )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< transform, rigidforce, rigidbody, rotation, mass, player_controller, camera, animator >();
		    Query.m_OneOf.AddFromComponents< paperback::component::entity, player_interaction >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera, animator& Animator, player_interaction* Interaction )
                {
                    if ( Controller.m_PlayerStatus && !Controller.m_FPSMode && Camera.m_Active )
                    {
                        auto DirectionalVector = Transform.m_Position - ConvertGLMVec3( Camera.m_Position );
                        DirectionalVector.y    = 0.0f;
                        auto Normalized        = DirectionalVector.Normalized();

                        // Not sure if we should use momentum, there seems to be no force cap
                        RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;

                        // Don't Override Jump Animations
                        if ( Controller.m_OnGround )
                        {
                            // Strong Unit
                            if ( Interaction )
                            {
                                if ( Interaction->m_bPushOrPull ) Animator.m_CurrentAnimationName = "StrongToy_Armature|PushWalk";
                                else                              Animator.m_CurrentAnimationName = "StrongToy_Armature|WalkStraightForward";

                                // Play Red Player Walk Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_RedWalk" );
                                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_PushMove = true;
                            }
                            // Jump Unit
                            else
                            {
                                Animator.m_CurrentAnimationName = "Armature|Walk";
                                
                                // Play Blue Player Walk Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_BlueWalk" );
                                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_JumpMove = true;
                            }

                            Animator.m_PlayOnce = false;
                        }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( Keyboard_EntityMovement_Backwards )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera, animator >();
            Query.m_OneOf.AddFromComponents< paperback::component::entity, player_interaction >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera, animator& Animator, player_interaction* Interaction )
                {
                    if ( Controller.m_PlayerStatus && !Controller.m_FPSMode && Camera.m_Active )
                    {
                        auto DirectionalVector = ConvertGLMVec3( Camera.m_Position ) - Transform.m_Position;
                        DirectionalVector.y    = 0.0f;
                        auto Normalized        = DirectionalVector.Normalized();

                        // Not sure if we should use momentum, there seems to be no force cap
                        RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;

                        // Don't Override Jump Animations
                        if ( Controller.m_OnGround )
                        {
                            // Strong Unit
                            if ( Interaction )
                            {
                                if ( Interaction->m_bPushOrPull ) Animator.m_CurrentAnimationName = "StrongToy_Armature|PullWalk";
                                else                              Animator.m_CurrentAnimationName = "StrongToy_Armature|WalkStraightBackward";

                                // Play Red Player Walk Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_RedWalk" );
                                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_PushMove = true;
                            }
                            // Jump Unit
                            else
                            {
                                Animator.m_CurrentAnimationName = "Armature|Walk";

                                // Play Blue Player Walk Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_BlueWalk" );
                                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_JumpMove = true;
                            }

                            Animator.m_PlayOnce = false;
                        }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT

    BEGIN_BINDING_CONSTRUCT( Keyboard_EntityMovement_Left )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera, animator >();
            Query.m_OneOf.AddFromComponents< paperback::component::entity, player_interaction >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera, animator& Animator, player_interaction* Interaction )
                {
                    if ( Interaction && Interaction->m_bPushOrPull ) return;
                    if ( Controller.m_PlayerStatus && !Controller.m_FPSMode && Camera.m_Active )
                    {
                        auto DirectionalVector = Transform.m_Position - ConvertGLMVec3( Camera.m_Position );
                        DirectionalVector.y    = 0.0f;
                        auto Normalized        = DirectionalVector.Normalized();

                        float x = Normalized.x *  cosf(90.0f) + Normalized.z * sinf(90.0f);
                        float z = Normalized.x * -sinf(90.0f) + Normalized.z * cosf(90.0f);

                        Normalized.x = x;
                        Normalized.z = z;

                        RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;

                        // Don't Override Jump Animations
                        if ( Controller.m_OnGround )
                        {
                            // Strong Unit
                            if ( Interaction )
                            {
                                Animator.m_CurrentAnimationName = "StrongToy_Armature|WalkStraightForward";

                                // Play Red Player Walk Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_RedWalk" );
                                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_PushMove = true;
                            }
                            // Jump Unit
                            else
                            {
                                Animator.m_CurrentAnimationName = "Armature|Walk";

                                // Play Blue Player Walk Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_BlueWalk" );
                                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_JumpMove = true;
                            }

                            Animator.m_PlayOnce = false;
                        }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT

    BEGIN_BINDING_CONSTRUCT( Keyboard_EntityMovement_Right )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera, animator >();
            Query.m_OneOf.AddFromComponents< paperback::component::entity, player_interaction >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera, animator& Animator, player_interaction* Interaction )
                {
                    if ( Interaction && Interaction->m_bPushOrPull ) return;
                    if ( Controller.m_PlayerStatus && !Controller.m_FPSMode && Camera.m_Active )
                    {
                        auto DirectionalVector = Transform.m_Position - ConvertGLMVec3( Camera.m_Position );
                        DirectionalVector.y    = 0.0f;
                        auto Normalized        = DirectionalVector.Normalized();

                        float x = Normalized.x *  cosf(-90.0f) + Normalized.z * sinf(-90.0f);
                        float z = Normalized.x * -sinf(-90.0f) + Normalized.z * cosf(-90.0f);

                        Normalized.x = x;
                        Normalized.z = z;

                        RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;

                        // Don't Override Jump Animations
                        if ( Controller.m_OnGround )
                        {
                            // Strong Unit
                            if ( Interaction )
                            {
                                Animator.m_CurrentAnimationName = "StrongToy_Armature|WalkStraightForward";

                                // Play Red Player Walk Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_RedWalk" );
                                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_PushMove = true;
                            }
                            // Jump Unit
                            else
                            {
                                Animator.m_CurrentAnimationName = "Armature|Walk";

                                // Play Blue Player Walk Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_BlueWalk" );
                                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_JumpMove = true;
                            }

                            Animator.m_PlayOnce = false;
                        }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT



    BEGIN_BINDING_CONSTRUCT( Gamepad_EntityMovement )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera, boundingbox >();
            Query.m_OneOf.AddFromComponents< paperback::component::entity, player_interaction >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                auto DebugSys = m_Coordinator.FindSystem<debug_system>();
                auto GP = m_Coordinator.FindGamepad();

                if ( GP && DebugSys )
                {
                    m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera, animator& Animator, mass& Mass, boundingbox& BB, player_interaction* Interaction )
                    {
                        if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && !Controller.m_FPSMode && Camera.m_Active )
                        {
                            auto Axes = GP->m_State.m_LeftAxis;

                            ///****************************/
                            ////      X-Axis Movement
                            ///****************************/
                            {
                                auto DirectionalVector = Transform.m_Position - ConvertGLMVec3( Camera.m_Position );
                                DirectionalVector.y    = 0.0f;
                                auto Normalized        = DirectionalVector.Normalized();

                                // Moving Left
                                if ( Axes.x > 0.0f && (!Interaction || Interaction && !Interaction->m_bPushOrPull) )
                                {
                                    float x      = Normalized.x *  cosf(-90.0f) + Normalized.z * sinf(-90.0f);
                                    float z      = Normalized.x * -sinf(-90.0f) + Normalized.z * cosf(-90.0f);
                                    Normalized.x = x;
                                    Normalized.z = z;

                                    RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
                                }
                                // Moving Right
                                else if ( Axes.x < 0.0f && (!Interaction || Interaction && !Interaction->m_bPushOrPull) )
                                {
                                    float x      = Normalized.x *  cosf(90.0f) + Normalized.z * sinf(90.0f);
                                    float z      = Normalized.x * -sinf(90.0f) + Normalized.z * cosf(90.0f);
                                    Normalized.x = x;
                                    Normalized.z = z;

                                    RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
                                }

                                // Currently Pushing / Pulling a select Entity
                                if ( Interaction && Interaction->m_bPushOrPull && 
                                     Interaction->m_InteractableGID != paperback::settings::invalid_index_v )
                                {
                                    // Find Entity That's Pushable Currently
                                    const auto& Info = m_Coordinator.GetEntityInfo( Interaction->m_InteractableGID );

                                    if ( Info.m_pArchetype )
                                    {
                                        // Reset Interactable Object Push Status
                                        auto [ InterRF, InterMass, InterBB, InterTransform ] = Info.m_pArchetype->FindComponents<rigidforce, mass, boundingbox, transform>( Info.m_PoolDetails );
                                        if ( InterRF && InterMass && InterBB && InterTransform )
                                        {
                                            auto AllowableDist = ( InterBB->Max + BB.Max ).MagnitudeSq() * 1.5f;
                                            auto Dist          = Transform.m_Position - InterTransform->m_Position;

                                            // If Within Some Set Distance Range
                                            if ( Dist.MagnitudeSq() >= AllowableDist )
                                            {
                                                // Reset Player Status
                                                Interaction->m_InteractableGID = settings::invalid_index_v;
                                                Interaction->m_bPushOrPull     = false;
                                            }
                                            else
                                            {
                                                InterRF->m_Momentum = RF.m_Momentum;
                                                InterRF->m_CollisionAffected = true;
                                                InterRF->m_GravityAffected = true;
                                                InterMass->m_Mass = Mass.m_Mass;
                                            }
                                        }
                                    }
                                }
                            }


                            /****************************/
                            //      Y-Axis Movement
                            /****************************/
                            {
                                // Moving Forward Or Backwards
                                auto DirectionalVector2 = Axes.y > 0.0f
                                                         ? ConvertGLMVec3( Camera.m_Position ) - Transform.m_Position
                                                         : Transform.m_Position - ConvertGLMVec3( Camera.m_Position );

                                DirectionalVector2.y    = 0.0f;
                                auto Normalized2        = DirectionalVector2.Normalized();

                                RF.m_Momentum += Normalized2 * Controller.m_MovementForce * Dt;
                            }



                            /****************************/
                            //     Update Animations
                            /****************************/
                            if ( Controller.m_OnGround && ( Axes.x > paperback::settings::gamepad_axis_ignore_v || Axes.x < -paperback::settings::gamepad_axis_ignore_v || 
                                                            Axes.y > paperback::settings::gamepad_axis_ignore_v || Axes.y < -paperback::settings::gamepad_axis_ignore_v ) )
                            {
                                // Strong Unit
                                if ( Interaction )
                                {
                                    if ( Interaction->m_bPushOrPull ) Animator.m_CurrentAnimationName = "StrongToy_Armature|PushWalk";
                                    else                              Animator.m_CurrentAnimationName = "StrongToy_Armature|WalkStraightForward";

                                    // Play Red Player Walk Sound
                                    m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_RedWalk" );
                                    m_Coordinator.GetSystem<onevent_ResetAnimation>().m_PushMove = true;
                                }
                                // Jump Unit
                                else
                                {
                                    Animator.m_CurrentAnimationName = "Armature|Walk";

                                    // Play Blue Player Walk Sound
                                    m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_BlueWalk" );
                                    m_Coordinator.GetSystem<onevent_ResetAnimation>().m_JumpMove = true;
                                }

                                Animator.m_PlayOnce = false;
                            }
                        }
                    });
                }
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    //-----------------------------------
    //     Camera Control Binding
    //-----------------------------------

    BEGIN_BINDING_CONSTRUCT( Mouse_Camera_Rotate )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rotation, mass, player_controller, camera >();
		    Query.m_OneOf.AddFromComponents<paperback::component::entity, player_interaction>();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, camera& Camera, player_interaction* Interaction )
                {
                    if ( Controller.m_PlayerStatus && Camera.m_Active && !m_Coordinator.GetPauseBool() )
                    {
                        if ( Interaction && Interaction->m_bPushOrPull ) return;

                        auto Direction = m_Coordinator.GetMouseDirection();

                        Direction = glm::normalize(Direction) * Controller.m_CameraRotationSpeed * m_Coordinator.GetMouseSensitivityRatio() * 0.01f;

	                    if ( Direction.x < 0 )
	                    {
                            Camera.RotateLeft( Direction.x * -1.f );
	                    }
	                    else if ( Direction.x > 0 )
	                    {
                            Camera.RotateRight( Direction.x );
	                    }

	                    if (Direction.y > 0)
	                    {
                            Camera.RotateDown( Direction.y );
	                    }
	                    else if (Direction.y < 0)
	                    {
                            Camera.RotateUp( Direction.y * -1.f );
	                    }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( Gamepad_Camera_Rotate )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rotation, mass, player_controller, camera >();
		    Query.m_OneOf.AddFromComponents<paperback::component::entity, player_interaction>();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                auto GP = m_Coordinator.FindGamepad();

                if ( GP )
                {
                    m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, camera& Camera, player_interaction* Interaction )
                    {
                        if ( Controller.m_PlayerStatus && Camera.m_Active && !m_Coordinator.GetPauseBool() )
                        {
                            if ( Interaction && Interaction->m_bPushOrPull ) return;
                            Camera.RotateRight( GP->m_State.m_RightAxis.x * Controller.m_CameraRotationSpeed * m_Coordinator.GetMouseSensitivityRatio() * 0.01f );

                            if ( GP->m_State.m_RightAxis.y > 0.0f )
                                Camera.RotateDown( std::fabs(GP->m_State.m_RightAxis.y * Controller.m_CameraRotationSpeed  * m_Coordinator.GetMouseSensitivityRatio() * 0.01f) );
                            else if ( GP->m_State.m_RightAxis.y < 0.0f )
                                Camera.RotateUp( std::fabs(GP->m_State.m_RightAxis.y * Controller.m_CameraRotationSpeed  * m_Coordinator.GetMouseSensitivityRatio() * 0.01f) );
                        }
                    });
                }
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    //-----------------------------------
    //          Player Action
    //-----------------------------------

    BEGIN_BINDING_CONSTRUCT( Jump_Action )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, animator, boundingbox, transform, paperback::component::entity >();
		    Query.m_NoneOf.AddFromComponents< prefab, player_interaction >();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, rigidforce& RF, rigidbody& RB, camera& Camera, animator& Animator, transform& Transform, boundingbox& BB, paperback::component::entity& Entity )
                {
                    if ( Controller.m_PlayerStatus && Controller.m_OnGround && !Controller.m_FPSMode && Camera.m_Active && !m_Coordinator.GetPauseBool() )
                    {
                        std::vector<std::pair<paperback::Vector3f, paperback::Vector3f>> RayList;
                        std::vector<paperback::u32>                                      ExcludeList;

                        auto Start_1 = Transform.m_Position; Start_1.x += BB.Min.x; Start_1.z += BB.Min.z;
                        auto Start_2 = Transform.m_Position; Start_2.x += BB.Min.x; Start_2.z -= BB.Min.z;
                        auto Start_3 = Transform.m_Position; Start_3.x -= BB.Min.x; Start_3.z += BB.Min.z;
                        auto Start_4 = Transform.m_Position; Start_4.x -= BB.Min.x; Start_4.z -= BB.Min.z;

                        auto RayEnd_1 = Transform.m_Position + ( paperback::Vector3f{ 0.0f, BB.Min.y, 0.0f } * 1.8f ) + paperback::Vector3f{  BB.Min.x, 0.0f,  BB.Min.z };
                        auto RayEnd_2 = Transform.m_Position + ( paperback::Vector3f{ 0.0f, BB.Min.y, 0.0f } * 1.8f ) + paperback::Vector3f{  BB.Min.x, 0.0f, -BB.Min.z };
                        auto RayEnd_3 = Transform.m_Position + ( paperback::Vector3f{ 0.0f, BB.Min.y, 0.0f } * 1.8f ) + paperback::Vector3f{ -BB.Min.x, 0.0f,  BB.Min.z };
                        auto RayEnd_4 = Transform.m_Position + ( paperback::Vector3f{ 0.0f, BB.Min.y, 0.0f } * 1.8f ) + paperback::Vector3f{ -BB.Min.x, 0.0f, -BB.Min.z };

                        RayList.push_back({ Start_1, RayEnd_1 });
                        RayList.push_back({ Start_2, RayEnd_2 });
                        RayList.push_back({ Start_3, RayEnd_3 });
                        RayList.push_back({ Start_4, RayEnd_4 });

                        ExcludeList.push_back( Entity.m_GlobalIndex );

                        auto [ Closest_GID, Distance ] = m_Coordinator.QueryMultipleRaycastClosest( RayList
                                                                                                  , Entity
                                                                                                  , Transform
                                                                                                  , std::fabs(BB.Min.y) * 1.2f
                                                                                                  , ExcludeList );

                        // Nothing within Acceptable Bounds
                        if ( Distance > Start_1.y - RayEnd_1.y )
                        {
                            //std::cout << "Closest Dist: " << Distance << "  |  Calculated Dist: " << Start_1.y - RayEnd_1.y << std::endl;
                            return;
                        }

                        Controller.m_OnGround = false;
                        RF.m_Momentum.y = ( 2.0f * Controller.m_JumpForce ) / 0.3f;

                        Animator.m_CurrentAnimationName = "Armature|JumpStart";
                        Animator.m_CurrentTime = 8.07f;
                        Animator.m_PlayOnce = true;

                        m_Coordinator.GetSystem<sound_system>().StopTriggeredSoundEvent("SFX_BlueWalk");

                        // Play Player Jump Sound
                        m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_BlueJump" );
                        m_Coordinator.GetSystem<onevent_ResetAnimation>().m_JumpMove = true;
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( Lift_Action )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, transform, player_interaction >();
		    Query.m_NoneOf.AddFromComponents< prefab >();

            // Game Is Not Paused
            if ( !m_Coordinator.GetPauseBool() )
            {
                // Find Player Entity - That Can Push / Pull
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, player_interaction& Interaction, player_controller& Controller
                                                                         , boundingbox& BB, mass& Mass, rigidforce& RF, rigidbody& RB ) -> bool
                {
                    if ( !Controller.m_PlayerStatus || Controller.m_FPSMode ) return false;

                    // Currently Pushing / Pulling a select Entity
                    if ( !Interaction.m_bPushOrPull && 
                         Interaction.m_InteractableGID == paperback::settings::invalid_index_v )
                    {
                        tools::query PQuery;
                        PQuery.m_Must.AddFromComponents< pushable, transform, mass, boundingbox, rigidforce >();
		                PQuery.m_NoneOf.AddFromComponents< prefab >();

                        //auto NeighbourList = m_Coordinator.QueryNeighbours( BB, Transform );

                        m_Coordinator.ForEach( m_Coordinator.Search(PQuery), [&]( paperback::component::entity& InterEntity, transform& InterTransform
                                                                                , mass& InterMass, boundingbox& InterBB, rigidforce& InterRF ) -> bool
                        {
                            auto AllowableDist = ( InterBB.Max + BB.Max ).MagnitudeSq() * 1.1f;
                            auto Dist          = Transform.m_Position - InterTransform.m_Position;

                            // If Within Some Set Distance Range
                            if ( Dist.MagnitudeSq() < AllowableDist )
                            {
                                // Update Interactable's Mass & Friction To Player's
                                InterMass.m_Mass = Mass.m_Mass;
                                InterRF.m_dynamicFriction = RF.m_dynamicFriction;
                                InterRF.m_CollisionAffected = true;
                                InterRF.m_GravityAffected = true;

                                // Reset Player Status
                                Interaction.m_InteractableGID = InterEntity.m_GlobalIndex;
                                Interaction.m_bPushOrPull     = true;

                                // Play Player Grab Interactable Sound
                                m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_RedGrab" );

                                // Found Interactable Nearby - *Note: ForEach Return Type
                                return true; // Return True - Terminates ForEach Loop Early
                            }

                            // Did Not Find Interactable Nearby - *Note: ForEach Return Type
                            return false; // Return False - Continue
                        });

                        // Found Player That Can Interact - *Note: ForEach Return Type
                        return true;
                    }
                    else
                    {
                        if ( RB.m_Velocity.y <= -0.1f ) return false;

                        // Find Entity That's Pushable Currently
                        const auto& Info = m_Coordinator.GetEntityInfo( Interaction.m_InteractableGID );

                        if ( Info.m_pArchetype )
                        {
                            // Reset Interactable Object Push Status
                            auto [ Mass, RF, InterRB ] = Info.m_pArchetype->FindComponents<mass, rigidforce, rigidbody>( Info.m_PoolDetails );
                            if ( InterRB && InterRB->m_Velocity.y <= -0.1f ) return false;
                            if ( Mass ) Mass->m_Mass = 0.0f;
                            if (RF)
                            {
                                RF->m_Momentum = paperback::Vector3f{};
                                RF->m_CollisionAffected = false;
                                RF->m_GravityAffected = false;
                            }
                            // Reset Player Status
                            Interaction.m_InteractableGID = paperback::settings::invalid_index_v;
                            Interaction.m_bPushOrPull     = false;
                        }
                    }

                    // Did Not Find Player That Can Interact - *Note: ForEach Return Type
                    return false;
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( Release_Action )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, transform, player_interaction >();
		    Query.m_NoneOf.AddFromComponents< prefab >();

            // Game Is Not Paused
            if ( !m_Coordinator.GetPauseBool() )
            {
                // Find Player Entity - That Can Push / Pull
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, player_interaction& Interaction, player_controller& Controller, boundingbox& BB, mass& Mass ) -> bool
                {
                    if ( Controller.m_FPSMode ) return false;

                    // Currently Pushing / Pulling a select Entity
                    if ( Interaction.m_bPushOrPull && 
                         Interaction.m_InteractableGID != paperback::settings::invalid_index_v )
                    {
                        // Find Entity That's Pushable Currently
                        const auto& Info = m_Coordinator.GetEntityInfo( Interaction.m_InteractableGID );

                        if ( Info.m_pArchetype )
                        {
                            // Reset Interactable Object Push Status
                            auto [ Mass, RF ] = Info.m_pArchetype->FindComponents<mass, rigidforce>( Info.m_PoolDetails );
                            if ( Mass ) Mass->m_Mass = 0.0f;
                            if (RF)
                            {
                                RF->m_Momentum = paperback::Vector3f{};
                                RF->m_CollisionAffected = false;
                                RF->m_GravityAffected = false;
                            }
                            // Reset Player Status
                            Interaction.m_InteractableGID = paperback::settings::invalid_index_v;
                            Interaction.m_bPushOrPull     = false;

                            return true;
                        }
                    }

                    return false;
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( PushPull_Action )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, transform, player_interaction, boundingbox >();
		    Query.m_NoneOf.AddFromComponents< prefab >();

            // Game Is Not Paused
            if ( !m_Coordinator.GetPauseBool() )
            {
                // Find Player Entity - That Can Push / Pull
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, player_interaction& Interaction, player_controller& Controller, rigidforce& RF, mass& Mass, boundingbox& BB ) -> bool
                {
                    if ( !Controller.m_PlayerStatus || Controller.m_FPSMode ) return false;

                    // Currently Pushing / Pulling a select Entity
                    if ( Interaction.m_bPushOrPull && 
                         Interaction.m_InteractableGID != paperback::settings::invalid_index_v )
                    {
                        // Find Entity That's Pushable Currently
                        const auto& Info = m_Coordinator.GetEntityInfo( Interaction.m_InteractableGID );

                        if ( Info.m_pArchetype )
                        {
                            // Reset Interactable Object Push Status
                            auto [ InterRF, InterMass, InterBB, InterTransform ] = Info.m_pArchetype->FindComponents<rigidforce, mass, boundingbox, transform>( Info.m_PoolDetails );
                            if ( InterRF && InterMass && InterBB && InterTransform )
                            {
                                auto AllowableDist = ( InterBB->Max + BB.Max ).MagnitudeSq() * 1.5f;
                                auto Dist          = Transform.m_Position - InterTransform->m_Position;

                                // If Within Some Set Distance Range
                                if ( Dist.MagnitudeSq() >= AllowableDist )
                                {
                                    // Reset Player Status
                                    Interaction.m_InteractableGID = settings::invalid_index_v;
                                    Interaction.m_bPushOrPull     = false;
                                }
                                else
                                {
                                    InterRF->m_Momentum = RF.m_Momentum * 1.4f;
                                    InterRF->m_CollisionAffected = true;
                                    InterRF->m_GravityAffected = true;
                                    InterMass->m_Mass = Mass.m_Mass;
                                }
                            }

                            return true;
                        }
                    }

                    return false;
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( Enable_FPS_Action )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, animator >();
		    Query.m_NoneOf.AddFromComponents< prefab >();

            tools::query CrosshairQuery;
            CrosshairQuery.m_Must.AddFromComponents< mesh, crosshair >();
		    CrosshairQuery.m_NoneOf.AddFromComponents< prefab >();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, camera& Camera, rigidbody& RB )
                {
                    if ( Controller.m_PlayerStatus && Controller.m_OnGround && Camera.m_Active && RB.m_Velocity.MagnitudeSq() < 0.1f )
                    {
                        if ( !Controller.m_FPSMode )
                        {
                            // Toggle FPS Crosshair - Enable
                            m_Coordinator.ForEach( m_Coordinator.Search( CrosshairQuery ), [&]( mesh& CrosshairMesh ) -> bool
                            {
                                if ( !CrosshairMesh.m_Active )
                                {
                                    // Backup Camera Radius | Update FPS Radius
                                    Controller.m_CameraRadius = Camera.m_Radius;
                                    Camera.m_Radius = 0.1f;

                                    // Enable Crosshair
                                    CrosshairMesh.m_Active = true;
                                    
                                    return true;
                                }

                                return false;
                            });

                            // Reset Camera Height Offset
                            Controller.m_FPSMode = true;

                            // Play FPS Mode Sound
                            m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_ToggleFPSMode" );
                        }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( Disable_FPS_Action )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, animator >();
		    Query.m_NoneOf.AddFromComponents< prefab >();

            tools::query CrosshairQuery;
            CrosshairQuery.m_Must.AddFromComponents< mesh, crosshair >();
		    CrosshairQuery.m_NoneOf.AddFromComponents< prefab >();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, camera& Camera, rigidbody& RB )
                {
                    if ( Controller.m_PlayerStatus && Controller.m_OnGround && Camera.m_Active && RB.m_Velocity.MagnitudeSq() < 0.1f )
                    {
                        if ( Controller.m_FPSMode )
                        {
                            // Toggle FPS Crosshair - Disable
                            m_Coordinator.ForEach( m_Coordinator.Search( CrosshairQuery ), [&]( mesh& CrosshairMesh ) -> bool
                            {
                                if ( CrosshairMesh.m_Active )
                                {
                                    // Restore Camera Radius
                                    Camera.m_Radius = Controller.m_CameraRadius;
                                    Controller.m_CameraRadius = 0.0f;

                                    // Disable Crosshair
                                    CrosshairMesh.m_Active = false;

                                    return true;
                                }

                                return false;
                            });

                            // Reset Camera Height Offset
                            Controller.m_FPSMode = false;

                            // Play FPS Mode Sound
                            m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_ToggleFPSMode" );
                        }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( Toggle_Players )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, animator >();
		    Query.m_OneOf.AddFromComponents< player_interaction, paperback::component::entity >();
		    Query.m_NoneOf.AddFromComponents< prefab >();

            bool ValidSwap = true;

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, camera& Camera, rigidforce& RF, rigidbody& RB, mass& Mass, player_interaction* Interaction ) -> bool
                {
                    // FPS Mode Is Active - Do Not Allow Player Swap
                    if ( Controller.m_PlayerStatus && Camera.m_Active )
                    {
                        // In FPS Mode / Strong Unit Falling While Pushing - Invalid Toggle
                        if ( Controller.m_FPSMode || (Interaction && Interaction->m_bPushOrPull && RB.m_Velocity.y <= -0.1f) )
                        {
                            ValidSwap = false;
                            return true;
                        }
                        if ( Interaction && Interaction->m_bPushOrPull )
                        {
                            // Find Entity That's Pushable Currently
                            const auto& Info = m_Coordinator.GetEntityInfo( Interaction->m_InteractableGID );

                            if ( Info.m_pArchetype )
                            {
                                // Reset Interactable Object Push Status
                                auto [ InterMass, InterRF ] = Info.m_pArchetype->FindComponents<mass, rigidforce>( Info.m_PoolDetails );

                                if ( InterMass && InterRF )
                                {
                                    InterRF->m_Momentum = paperback::Vector3f{};

                                    // Reset Player Push/Pull Status
                                    Interaction->m_bPushOrPull = false;
                                    Interaction->m_InteractableGID = settings::invalid_index_v;
                                }
                            }
                        }
                    }

                    return false;
                });

                if ( ValidSwap )
                {
                    m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, camera& Camera )
                    {
                        if ( Controller.m_PlayerStatus && Camera.m_Active )
                        {
                            Controller.m_PlayerStatus = Camera.m_Active = false;
                        }
                        else if ( !Controller.m_PlayerStatus && !Camera.m_Active )
                        {
                            Controller.m_PlayerStatus = Camera.m_Active = true;
                        }
                    });

                    // Play Toggle Player Sound
                    m_Coordinator.GetSystem<sound_system>().TriggerTaggedSound( "SFX_TogglePlayer" );
                }
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    //-----------------------------------
    //         Window Bindings
    //-----------------------------------

    BEGIN_BINDING_CONSTRUCT( Toggle_Fullscreen )
        BEGIN_INPUT_ACTION

            auto WinSystem = m_Coordinator.FindSystem<window_system>();
            if ( WinSystem ) WinSystem->FullScreen();

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT
}

#endif