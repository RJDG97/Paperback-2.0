#pragma once

struct mousesens_decrease_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Mouse Sensitivity Decrease Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    mousesens_decrease_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
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

        float sens = PPB.GetMouseSensitivityRatio();

        if (sens > 0.2f)
        {

            sens -= 0.2f;
            PPB.SetMouseSensitivityRatio(sens);
        }
    }
};