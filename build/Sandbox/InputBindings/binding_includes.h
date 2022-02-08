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
                    if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
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

                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_RedWalk");
                            }
                            // Jump Unit
                            else
                            {
                                Animator.m_CurrentAnimationName = "Armature|Walk";
                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_BlueWalk");
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
                    if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
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

                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_RedWalk");
                            }
                            // Jump Unit
                            else
                            {
                                Animator.m_CurrentAnimationName = "Armature|Walk";

                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_BlueWalk");
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
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera, animator, animator >();
            Query.m_OneOf.AddFromComponents< paperback::component::entity, player_interaction >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera, animator& Animator, player_interaction* Interaction )
                {
                    if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
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

                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_RedWalk");
                            }
                            // Jump Unit
                            else
                            {
                                Animator.m_CurrentAnimationName = "Armature|Walk";

                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_BlueWalk");
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
                    if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
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

                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_RedWalk");
                            }
                            // Jump Unit
                            else
                            {
                                Animator.m_CurrentAnimationName = "Armature|Walk";

                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_BlueWalk");
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
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera >();
            Query.m_OneOf.AddFromComponents< paperback::component::entity, player_interaction >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                auto DebugSys = m_Coordinator.FindSystem<debug_system>();
                auto GP = m_Coordinator.FindGamepad();

                if ( GP && DebugSys )
                {
                    m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera, animator& Animator, player_interaction* Interaction )
                    {
                        if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
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
                                if ( Axes.x > 0.0f )
                                {
                                    float x      = Normalized.x *  cosf(-90.0f) + Normalized.z * sinf(-90.0f);
                                    float z      = Normalized.x * -sinf(-90.0f) + Normalized.z * cosf(-90.0f);
                                    Normalized.x = x;
                                    Normalized.z = z;

                                    RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
                                }
                                // Moving Right
                                else if ( Axes.x < 0.0f )
                                {
                                    float x      = Normalized.x *  cosf(90.0f) + Normalized.z * sinf(90.0f);
                                    float z      = Normalized.x * -sinf(90.0f) + Normalized.z * cosf(90.0f);
                                    Normalized.x = x;
                                    Normalized.z = z;

                                    RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
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
                        }


                        // Yet To Add/Test Animation For Gamepad
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
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, camera& Camera )
                {
                    if ( Controller.m_PlayerStatus && Camera.m_Active && !m_Coordinator.GetPauseBool() )
                    {
                        auto Direction = m_Coordinator.GetMouseDirection();

                        Direction = glm::normalize(Direction) * Controller.m_CameraRotationSpeed * 0.01f;

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
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                auto GP = m_Coordinator.FindGamepad();

                if ( GP )
                {
                    m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, camera& Camera )
                    {
                        if ( Controller.m_PlayerStatus && Camera.m_Active && !m_Coordinator.GetPauseBool() )
                        {
                            Camera.RotateRight( GP->m_State.m_RightAxis.x * Controller.m_CameraRotationSpeed * 0.01f );
                            Camera.RotateDown( GP->m_State.m_RightAxis.y * Controller.m_CameraRotationSpeed * 0.01f );
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
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, animator >();
		    Query.m_NoneOf.AddFromComponents< prefab, player_interaction >();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, rigidforce& RF, rigidbody& RB, camera& Camera, animator& Animator )
                {
                    if ( Controller.m_PlayerStatus && Controller.m_OnGround && Camera.m_Active && !m_Coordinator.GetPauseBool() )
                    {
                        Controller.m_OnGround = false;
                        RF.m_Momentum.y = ( 2.0f * Controller.m_JumpForce ) / 0.3f;

                        Animator.m_CurrentAnimationName = "Armature|JumpStart";
                        Animator.m_CurrentTime = 8.07f;
                        Animator.m_PlayOnce = true;

                        auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                        if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_BlueJump");
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
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, player_interaction& Interaction, boundingbox& BB, mass& Mass, rigidforce& RF ) -> bool
                {
                    // Currently Pushing / Pulling a select Entity
                    if ( !Interaction.m_bPushOrPull && 
                         Interaction.m_InteractableGID == paperback::settings::invalid_index_v )
                    {
                        tools::query PQuery;
                        PQuery.m_Must.AddFromComponents< pushable, transform, mass, boundingbox, rigidforce >();
		                PQuery.m_NoneOf.AddFromComponents< prefab >();

                        m_Coordinator.ForEach( m_Coordinator.Search( PQuery ), [&]( paperback::component::entity& InterEntity, transform& InterTransform, mass& InterMass, boundingbox& InterBB, rigidforce& InterRF ) -> bool
                        {
                            //std::cout << "Trying To Pick..." << std::endl;
                            auto AllowableDist = ( InterBB.Max + BB.Max ).MagnitudeSq() * 1.1f;
                            auto Dist          = Transform.m_Position - InterTransform.m_Position;

                            // If Within Some Set Distance Range
                            if ( Dist.MagnitudeSq() < AllowableDist )
                            {
                                // Update Interactable's Mass & Friction To Player's
                                InterMass.m_Mass = Mass.m_Mass;
                                InterRF.m_dynamicFriction = RF.m_dynamicFriction;

                                // Reset Player Status
                                Interaction.m_InteractableGID = InterEntity.m_GlobalIndex;
                                Interaction.m_bPushOrPull     = true;

                                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                                if (UI_Sys) UI_Sys->TriggerSoundEntity("SFX_RedGrab");

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
                        // Find Entity That's Pushable Currently
                        const auto& Info = m_Coordinator.GetEntityInfo( Interaction.m_InteractableGID );

                        if ( Info.m_pArchetype )
                        {
                            // Reset Interactable Object Push Status
                            auto [ Mass, RF ] = Info.m_pArchetype->FindComponents<mass, rigidforce>( Info.m_PoolDetails );
                            if ( Mass ) Mass->m_Mass = 0.0f;
                            if ( RF ) RF->m_Momentum = paperback::Vector3f{};

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
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, player_interaction& Interaction, boundingbox& BB, mass& Mass )
                {
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
                            if ( RF ) RF->m_Momentum = paperback::Vector3f{};

                            // Reset Player Status
                            Interaction.m_InteractableGID = paperback::settings::invalid_index_v;
                            Interaction.m_bPushOrPull     = false;
                        }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( PushPull_Action )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera, transform, player_interaction >();
		    Query.m_NoneOf.AddFromComponents< prefab >();

            // Game Is Not Paused
            if ( !m_Coordinator.GetPauseBool() )
            {
                // Find Player Entity - That Can Push / Pull
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, player_interaction& Interaction, rigidforce& RF )
                {
                    // Currently Pushing / Pulling a select Entity
                    if ( Interaction.m_bPushOrPull && 
                         Interaction.m_InteractableGID != paperback::settings::invalid_index_v )
                    {
                        // Find Entity That's Pushable Currently
                        const auto& Info = m_Coordinator.GetEntityInfo( Interaction.m_InteractableGID );

                        if ( Info.m_pArchetype )
                        {
                            // Reset Interactable Object Push Status
                            auto InterRF = Info.m_pArchetype->FindComponent<rigidforce>( Info.m_PoolDetails );
                            if ( InterRF ) InterRF->m_Momentum = RF.m_Momentum;
                        }
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT
}

#endif