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
        };

        PPB_INLINE
        GamepadState( void ) noexcept
        {
            for ( int i = 0 ; i < GLFW_JOYSTICK_LAST; ++i )
            {
                if ( glfwJoystickPresent(i) )
                {
                    m_ControllerID = i;
                    std::cout << "Gamepad Device Connected During Initialization: " << m_ControllerID << std::endl;
                    return;
                }
                //std::cout << "Checking Active Devices..." << std::endl;
            }
        }

        PPB_INLINE
        void UpdateGamepadCurrentButtons( const unsigned char* ButtonValues ) noexcept
        {
            if ( ButtonValues )
            {
                for ( int i = 0; i < GLFW_GAMEPAD_BUTTON_LAST; ++i )
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
        }

        std::array< bool, GLFW_GAMEPAD_BUTTON_LAST >               m_Previous{ GLFW_RELEASE };                        // Previous Keyboard State
        std::array< bool, GLFW_GAMEPAD_BUTTON_LAST >               m_Current{ GLFW_RELEASE };                         // Current Keyboard State
        std::array< action::instance, GLFW_GAMEPAD_BUTTON_LAST >   m_Actions{ };                                      // Mouse Action Bindings

        Vec2                                                       m_LeftAxis{ };
        Vec2                                                       m_RightAxis{ };
        float                                                      m_LeftTrigger = 0.0f;
        float                                                      m_RightTrigger = 0.0f;

        int                                                        m_ControllerID = settings::invalid_controller_v;   // Active Controller Index - Updated Using Callback
    };
}

#endif