#pragma once

struct levelselect_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "LevelSelect Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    levelselect_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
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
        PPB.QueueScene("LevelSelect");
    }
};