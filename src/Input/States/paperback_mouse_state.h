#pragma once

#ifndef PPB_MOUSE_STATE
#define PPB_MOUSE_STATE

namespace paperback::input
{
    struct MouseState
    {
        std::array< bool, GLFW_MOUSE_BUTTON_LAST >               m_Previous{ GLFW_RELEASE };               // Previous Mouse State
        std::array< bool, GLFW_MOUSE_BUTTON_LAST >               m_Current{ GLFW_RELEASE };                // Current Mouse State
        std::array< action::instance, GLFW_MOUSE_BUTTON_LAST >   m_Actions{ };                             // Mouse Action Bindings

        glm::vec2                                                m_MouseOriginPosition{};                  // Mouse Origin
        double                                                   m_YAxis{};                                // Scroll Wheel
    };
}

#endif