#pragma once

struct restartlevel_button_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Restart Level Button Script"
    };

    // You have to define this constructor - For internal registration
    restartlevel_button_script(paperback::coordinator::instance& Instance) noexcept :
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
        PPB.TogglePause(false);

        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PAUSE), false);
        PPB.QueueScene("Reload");
    }
};