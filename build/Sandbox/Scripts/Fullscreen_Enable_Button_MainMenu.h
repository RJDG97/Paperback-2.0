#pragma once

struct fullscreen_enable_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Fullscreen Enable Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    fullscreen_enable_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
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

        PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");

        PPB.GetSystem<window_system>().FullScreen();

        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::FULLSCREEN_OFF), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::FULLSCREEN_ON), true);
    }
};