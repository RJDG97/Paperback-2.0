#pragma once
#include "../../Sandbox/Systems/UISystem.h"

struct quitcancel_button_game_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Quit Cancel Button Game Script"
    };

    // You have to define this constructor - For internal registration
    quitcancel_button_game_script(paperback::coordinator::instance& Instance) noexcept :
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
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::QUIT), false);
        PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PAUSE), true);
    }
};