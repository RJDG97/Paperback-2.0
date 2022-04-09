#pragma once

struct volume_decrease_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Volume Decrease Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    volume_decrease_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
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

        PPB.GetSystem<sound_system>().DecreaseVolume();

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