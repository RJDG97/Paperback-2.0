/**********************************************************************************
*\file         SplashScreen_Mode.h
*\brief        SplashScreen_Mode.h
*
*\author       Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct splashscreen_mode_script : paperback::script::button_interface // Inherited Type (1)
{

    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Splash Screen Mode Script"
    };

    // You have to define this constructor - For internal registration
    splashscreen_mode_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    void Run( void ) noexcept
    {
        if (PPB.IsKeyPress(GLFW_KEY_ESCAPE) || PPB.IsKeyPress(GLFW_KEY_ENTER) || PPB.IsKeyPress(GLFW_KEY_TAB) || PPB.IsKeyPress(GLFW_KEY_SPACE) 
            || PPB.IsMousePress(GLFW_MOUSE_BUTTON_1) || PPB.IsMousePress(GLFW_MOUSE_BUTTON_2)
            || PPB.IsGamepadButtonPressDown(7) || PPB.IsGamepadButtonPressDown(0) || PPB.IsGamepadButtonPressDown(1)) // start, A, B button
        {

            PPB.QueueScene("MainMenu");
        }
    }
};