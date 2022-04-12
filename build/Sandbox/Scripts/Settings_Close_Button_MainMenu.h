#pragma once

struct settings_close_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Settings Close Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    settings_close_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
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

        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::SETTINGS), false);

        for (size_t layer = UI_LAYER::VOLUME1; layer <= UI_LAYER::VOLUME5; ++layer)
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(layer), false);
        }

        for (size_t layer = UI_LAYER::MOUSESENS1; layer <= UI_LAYER::MOUSESENS5; ++layer)
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(layer), false);
        }

        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::FULLSCREEN_ON), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::FULLSCREEN_OFF), false);

        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::GAMMA_ON), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::GAMMA_OFF), false);

        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PLAYUI), true);
        PPB.GetSystem<ui_system>().UpdateMaximumIndex();
    }
};