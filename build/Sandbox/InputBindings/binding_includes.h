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
            Query.m_Must.AddFromComponents< transform, rigidforce, rigidbody, rotation, mass, player_controller, camera >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera )
                {
                    if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
                    {
                        auto DirectionalVector = Transform.m_Position - ConvertGLMVec3( Camera.m_Position );
                        auto Normalized        = DirectionalVector.Normalized();
                        Normalized.y           = 0.0f;

                        // Not sure if we should use momentum, there seems to be no force cap
                        RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    BEGIN_BINDING_CONSTRUCT( Keyboard_EntityMovement_Backwards )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera )
                {
                    if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
                    {
                        auto DirectionalVector = ConvertGLMVec3( Camera.m_Position ) - Transform.m_Position;
                        auto Normalized        = DirectionalVector.Normalized();
                        Normalized.y           = 0.0f;

                        // Not sure if we should use momentum, there seems to be no force cap
                        RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT

    BEGIN_BINDING_CONSTRUCT( Keyboard_EntityMovement_Left )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera )
                {
                    if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
                    {
                        auto DirectionalVector = Transform.m_Position - ConvertGLMVec3( Camera.m_Position );
                        auto Normalized        = DirectionalVector.Normalized();
                        Normalized.y           = 0.0f;

                        float x = Normalized.x *  cosf(90.0f) + Normalized.z * sinf(90.0f);
                        float z = Normalized.x * -sinf(90.0f) + Normalized.z * cosf(90.0f);

                        Normalized.x = x;
                        Normalized.z = z;

                        RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT

    BEGIN_BINDING_CONSTRUCT( Keyboard_EntityMovement_Right )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< transform, rigidforce, rotation, mass, player_controller, camera >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera )
                {
                    if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
                    {
                        auto DirectionalVector = Transform.m_Position - ConvertGLMVec3( Camera.m_Position );
                        auto Normalized        = DirectionalVector.Normalized();
                        Normalized.y           = 0.0f;

                        float x = Normalized.x *  cosf(-90.0f) + Normalized.z * sinf(-90.0f);
                        float z = Normalized.x * -sinf(-90.0f) + Normalized.z * cosf(-90.0f);

                        Normalized.x = x;
                        Normalized.z = z;

                        RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
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
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                auto DebugSys = m_Coordinator.FindSystem<debug_system>();
                auto GP = m_Coordinator.FindGamepad();

                if ( GP && DebugSys )
                {
                    m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, rigidforce& RF, player_controller& Controller, camera& Camera )
                    {
                        if ( Controller.m_PlayerStatus /*&& Controller.m_OnGround*/ && Camera.m_Active )
                        {
                            auto DirectionalVector = ConvertGLMVec3( Camera.m_Position ) - Transform.m_Position;
                            auto Normalized        = DirectionalVector.Normalized();
                            Normalized.y           = 0.0f;

                            // some rotation thing

                            RF.m_Momentum += Normalized * Controller.m_MovementForce * Dt;
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
                            Camera.RotateRight( GP->m_State.m_RightAxis.x * Controller.m_CameraRotationSpeed );
                            Camera.RotateDown( GP->m_State.m_RightAxis.y * Controller.m_CameraRotationSpeed );
                        }
                    });
                }
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT


    //-----------------------------------
    //           Jump Action
    //-----------------------------------

    BEGIN_BINDING_CONSTRUCT( Jump_Action )
        BEGIN_INPUT_ACTION

            // TODO - Update Query Initialization To Constructor Call
            tools::query Query;
            Query.m_Must.AddFromComponents< rigidforce, rigidbody, rotation, mass, player_controller, camera >();
		    Query.m_NoneOf.AddFromComponents<prefab>();

            if ( !m_Coordinator.GetPauseBool() )
            {
                m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( player_controller& Controller, rigidforce& RF, rigidbody& RB, camera& Camera )
                {
                    if ( Controller.m_PlayerStatus && Controller.m_OnGround && Camera.m_Active && !m_Coordinator.GetPauseBool() )
                    {
                        Controller.m_OnGround = false;
                        RF.m_Momentum.y = ( 2.0f * Controller.m_JumpForce ) / 0.3f;
                    }
                });
            }

        END_INPUT_ACTION
    END_BINDING_CONSTRUCT
}

#endif