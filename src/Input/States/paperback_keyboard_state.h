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