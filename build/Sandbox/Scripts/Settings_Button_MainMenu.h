/**********************************************************************************
*\file         Settings_Button_MainMenu.h
*\brief        Settings_Button_MainMenu.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct settings_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Settings Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    settings_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    // Override virtual functions defined within button_interface
    void OnHover(void) noexcept override
    {
    }

    void OnClick() noexcept override
    {

        PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");

        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PLAYUI), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::SETTINGS), true);

        float volume = PPB.GetSystem<sound_system>().GetVolumePercentage();
        size_t layer = UI_LAYER::VOLUME1;

        for (float vol = 0.0f; vol < volume; vol += 0.2f)
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(layer++), true);
        }
        
        float mousesens = PPB.GetMouseSensitivityRatio();
        layer = UI_LAYER::MOUSESENS1;

        for (float sens = 0.0f; sens < mousesens; sens += 0.2f)
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(layer++), true);
        }

        if (PPB.GetSystem<window_system>().GetFullScreen())
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::FULLSCREEN_ON), true);
        }
        else
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::FULLSCREEN_OFF), true);
        }

        if (PPB.GetSystem<render_system>().GetGamma())
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::GAMMA_ON), true);
        }
        else
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::GAMMA_OFF), true);
        }
        
        PPB.GetSystem<ui_system>().UpdateMaximumIndex();
    }
};