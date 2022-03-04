#pragma once

struct game_mode_script : paperback::script::button_interface // Inherited Type (1)
{

    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Game Mode Script"
    };

    // You have to define this constructor - For internal registration
    game_mode_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    void Run( void ) noexcept
    {
        if (PPB.IsKeyPress(GLFW_KEY_ESCAPE))
        {

            if (!PPB.GetPauseBool())
            {
                //PPB.GetSystem<ui_system>().TriggerSoundEntity("ButtonClickSFX");
                PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");
                PPB.TogglePause(true);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PLAYUI), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PAUSE), true);
            }
            else
            {

                //PPB.GetSystem<ui_system>().TriggerSoundEntity("ButtonClickSFX");
                PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");
                PPB.TogglePause(false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PAUSE), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PLAYUI), true);
            }
        }
    }
};