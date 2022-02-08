#pragma once

#ifndef PPB_MOUSE_STATE
#define PPB_MOUSE_STATE

namespace paperback::input
{
    struct MouseState
    {
        PPB_INLINE
        void UpdateMouseCoordinates( void ) noexcept
        {
            auto Handle = PPB.GetWindowHandle();

            if ( Handle )
            {
                double X, Y;
			    glfwGetCursorPos( Handle, &X, &Y );

			    m_PrevPos = m_CurrPos;
			    m_CurrPos = glm::vec2{ X, Y };

                auto Displacement = m_CurrPos - m_PrevPos;
                if ( Displacement.x > 0 || Displacement.y > 0 ||
                     Displacement.x < 0 || Displacement.y < 0 )
                {
                    // Assign Default Mouse Movement Binding Key
                    m_Current[ GLFW_MOUSE_BUTTON_3 ] = true;
                }
            }
        }


        std::array< bool, GLFW_MOUSE_BUTTON_LAST >               m_Previous{ GLFW_RELEASE };               // Previous Mouse State
        std::array< bool, GLFW_MOUSE_BUTTON_LAST >               m_Current{ GLFW_RELEASE };                // Current Mouse State
        std::array< action::instance, GLFW_MOUSE_BUTTON_LAST >   m_Actions{ };                             // Mouse Action Bindings

        glm::vec2                                                m_PrevPos{};                              // Previous Pos
        glm::vec2                                                m_CurrPos{};                              // Curr Pos

        double                                                   m_YAxis{};                                // Scroll Wheel
    };
}

#endif