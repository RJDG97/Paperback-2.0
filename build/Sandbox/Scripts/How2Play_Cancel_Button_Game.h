/**********************************************************************************
*\file         How2Play_Cancel_Button_Game.h
*\brief        How2Play_Cancel_Button_Game.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct how2play_cancel_button_game_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "How To Play Cancel Button Game Script"
    };

    // You have to define this constructor - For internal registration
    how2play_cancel_button_game_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    // Override virtual functions defined within button_interface
    void OnHover(void) noexcept override
    {
    }

    void OnClick() noexcept override
    {

        PPB.GetSystem<ui_system>().TriggerSoundEntity("ButtonClickSFX");
        
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY1), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY2), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY3), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY4), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY5), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY6), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY7), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PAUSE), true);
        PPB.GetSystem<ui_system>().UpdateMaximumIndex();
    }
};