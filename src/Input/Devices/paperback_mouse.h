#pragma once

#ifndef PPB_MOUSE_CONTROLS
#define PPB_MOUSE_CONTROLS

namespace paperback::input::device
{
    BEGIN_CONTROLLER_CONSTRUCT( Mouse_Controls
                              , device::type::mouse )

        PPB_INLINE
        void Update( const float Dt ) noexcept override
        {
            
        }

        PPB_INLINE
        void Reset( void ) noexcept
        {
            
        }    

        MouseState m_State;

    END_CONTROLLER_CONSTRUCT
}

#endif