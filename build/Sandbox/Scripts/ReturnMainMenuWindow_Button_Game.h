/**********************************************************************************
*\file         ReturnMainMenuWindow_Button_Game.h
*\brief        ReturnMainMenuWindow_Button_Game.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "../../Sandbox/Systems/UISystem.h"

struct returnmainmenu_window_button_game_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Return Main Menu Window Button Game Script"
    };

    // You have to define this constructor - For internal registration
    returnmainmenu_window_button_game_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    // Override virtual functions defined within button_interface
    void OnHover(void) noexcept override
    {
    }

    void OnClick() noexcept override
    {

        //PPB.GetSystem<ui_system>().TriggerSoundEntity("ButtonClickSFX");
        PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PAUSE), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::RETURNMAINMENU), true);
        PPB.GetSystem<ui_system>().UpdateMaximumIndex();
    }
};