/**********************************************************************************
*\file         paperback_keyboard_state.h
*\brief        paperback_keyboard_state.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#ifndef PPB_KEYBOARD_STATE
#define PPB_KEYBOARD_STATE

namespace paperback::input
{
    struct KeyboardState
    {
        std::array< bool, GLFW_KEY_LAST >                 m_Previous{ GLFW_RELEASE };            // Previous Keyboard State
        std::array< bool, GLFW_KEY_LAST >                 m_Current{ GLFW_RELEASE };             // Current Keyboard State
        std::array< action::instance, GLFW_KEY_LAST >     m_Actions{};                           // Key Action Bindings
    };
}

#endif