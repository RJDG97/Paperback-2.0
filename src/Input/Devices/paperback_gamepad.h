#pragma once

#ifndef PPB_GAMEPAD_CONTROLS
#define PPB_GAMEPAD_CONTROLS

namespace paperback::input::device
{
    BEGIN_CONTROLLER_CONSTRUCT( Gamepad_Controls
                              , device::type::gamepad )

        PPB_INLINE
        void Update( const float Dt ) noexcept override
        {
            
        }

        void DeviceStatusCallback( int ActiveDeviceIndex, int DeviceStatus )
        {
            switch ( DeviceStatus )
            {
                case GLFW_CONNECTED:
                {
                    m_State.m_ControllerID = ActiveDeviceIndex;
                    INFO_PRINT( ("Gamepad Device Connected: " + std::to_string( ActiveDeviceIndex )).c_str() );
                    break;
                }
                case GLFW_DISCONNECTED:
                {
                    m_State.m_ControllerID = settings::invalid_controller_v;
                    WARN_PRINT( ("Gamepad Device Disconnected: " + std::to_string( ActiveDeviceIndex )).c_str() );
                    break;
                }
            }
        }

        PPB_INLINE
        void Reset( void ) noexcept
        {
            
        }

        GamepadState m_State;

    END_CONTROLLER_CONSTRUCT
}

#endif