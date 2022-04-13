/**********************************************************************************
*\file         paperback_keyboard.h
*\brief        paperback_keyboard.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#ifndef PPB_KEYBOARD_CONTROLS
#define PPB_KEYBOARD_CONTROLS

namespace paperback::input::device
{
    BEGIN_CONTROLLER_CONSTRUCT( Keyboard_Controls
                              , device::type::keyboard )

        PPB_INLINE
        void Update( const float Dt ) noexcept override
        {
            
        }

        PPB_INLINE
        void Reset( void ) noexcept
        {
            
        }

        KeyboardState m_State;

    END_CONTROLLER_CONSTRUCT
}

#endif