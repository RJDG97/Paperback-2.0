/**********************************************************************************
*\file         paperback_gamepad.h
*\brief        paperback_gamepad.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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