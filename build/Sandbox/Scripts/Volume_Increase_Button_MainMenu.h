/**********************************************************************************
*\file         Volume_Increase_Button_MainMenu.h
*\brief        Volume_Increase_Button_MainMenu.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct volume_increase_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Volume Increase Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    volume_increase_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
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

        PPB.GetSystem<sound_system>().IncreaseVolume();

        for (size_t layer = UI_LAYER::VOLUME1; layer <= UI_LAYER::VOLUME5; ++layer)
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(layer), false);
        }

        size_t layer = UI_LAYER::VOLUME1;
        float volume = PPB.GetSystem<sound_system>().GetVolumePercentage();

        for (float vol = 0.0f; vol < volume; vol += 0.2f)
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(layer++), true);
        }
    }
};