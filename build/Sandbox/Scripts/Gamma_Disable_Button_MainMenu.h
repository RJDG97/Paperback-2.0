#pragma once

struct gamma_enable_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Gamma Enable Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    gamma_enable_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
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

        PPB.GetSystem<render_system>().SetGamma();

        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::GAMMA_OFF), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::GAMMA_ON), true);
    }
};