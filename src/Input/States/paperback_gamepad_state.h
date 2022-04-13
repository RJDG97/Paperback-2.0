/**********************************************************************************
*\file         paperback_gamepad_state.h
*\brief        paperback_gamepad_state.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#ifndef PPB_GAMEPAD_STATE
#define PPB_GAMEPAD_STATE

namespace paperback::input
{
    struct GamepadState
    {
        struct Vec2
        {
            float x = 0.0f
                , y = 0.0f;

            PPB_INLINE
            bool Valid( void ) noexcept
            {
                if ( x < paperback::settings::gamepad_axis_ignore_v && x > -paperback::settings::gamepad_axis_ignore_v ) x = 0.0f;
                if ( y < paperback::settings::gamepad_axis_ignore_v && y > -paperback::settings::gamepad_axis_ignore_v ) y = 0.0f;

                if ( x != 0.0f || y != 0.0f ) return true;
                else                          return false;
            }
        };

        PPB_INLINE
        GamepadState( void ) noexcept
        {
            
        }

        PPB_INLINE
        void UpdateGamepadCurrentButtons( const unsigned char* ButtonValues ) noexcept
        {
            if ( ButtonValues )
            {
                for ( int i = 0; i < GLFW_GAMEPAD_BUTTON_LAST + 1; ++i )
                {
                    m_Current[ i ] = std::bit_cast<bool>( ButtonValues[ i ] );
                }
            }
        }

        PPB_INLINE
        void UpdateGamepadAxis( const float* AxisValues ) noexcept
        {
            if ( AxisValues )
            {
                // Left Axis
                m_LeftAxis.x   = AxisValues[ 0 ];
                m_LeftAxis.y   = AxisValues[ 1 ];
                // Right Axis
                m_RightAxis.x  = AxisValues[ 2 ];
                m_RightAxis.y  = AxisValues[ 3 ];
                // Trigger Buttons
                m_LeftTrigger  = AxisValues[ 4 ];
                m_RightTrigger = AxisValues[ 5 ];
            }

            if ( m_LeftAxis.Valid() )
            {
                m_Current[ GLFW_GAMEPAD_BUTTON_LEFT_THUMB ] = true;
            }

            if ( m_RightAxis.Valid() )
            {
                m_Current[ GLFW_GAMEPAD_BUTTON_RIGHT_THUMB ] = true;
            }
        }

        PPB_INLINE
        void FindActiveController( void ) noexcept
        {
            for ( int i = 0 ; i < GLFW_JOYSTICK_LAST; ++i )
            {
                if ( glfwJoystickPresent(i) )
                {
                    m_ControllerID = i;
                    INFO_PRINT( ( "Active Gamepad Device Detected: GLFW_JOYSTICK_" + std::to_string(m_ControllerID) ).c_str() );
                    return;
                }
            }
            WARN_PRINT( "NO Active Gamepad Device Detected" );
        }

        std::array< bool, GLFW_JOYSTICK_LAST >               m_Previous{ GLFW_RELEASE };                        // Previous Keyboard State
        std::array< bool, GLFW_JOYSTICK_LAST >               m_Current{ GLFW_RELEASE };                         // Current Keyboard State
        std::array< action::instance, GLFW_JOYSTICK_LAST >   m_Actions{ };                                      // Mouse Action Bindings

        Vec2                                                 m_LeftAxis{ };
        Vec2                                                 m_RightAxis{ };
        float                                                m_LeftTrigger = 0.0f;
        float                                                m_RightTrigger = 0.0f;

        int                                                  m_ControllerID = settings::invalid_controller_v;   // Active Controller Index - Updated Using Callback
    };
}

#endif