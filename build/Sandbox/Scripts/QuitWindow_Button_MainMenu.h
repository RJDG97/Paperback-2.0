/**********************************************************************************
*\file         QuitWindow_Button_MainMenu.h
*\brief        QuitWindow_Button_MainMenu.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "../../Sandbox/Systems/UISystem.h"

struct quitcancel_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Quit Cancel Button Main Menu Script"
    };

    // You have to define this constructor - For internal registration
    quitcancel_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    // Override virtual functions defined within button_interface
    void OnHover(void) noexcept override
    {
    }

    void OnClick() noexcept override
    {

        if (!PPB.GetSystem<ui_system>().SetButtonLock())
            return;

        //PPB.GetSystem<ui_system>().TriggerSoundEntity("ButtonClickSFX");
        PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::QUIT), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PLAYUI), true);
        PPB.GetSystem<ui_system>().UpdateMaximumIndex();
    }
};