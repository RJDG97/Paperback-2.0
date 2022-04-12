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
        if (PPB.IsKeyPress(GLFW_KEY_ESCAPE) || PPB.IsGamepadButtonPressDown(7) || PPB.GetTabPaused()) // start button
        {

            if (!PPB.GetPauseBool() || PPB.GetTabPaused())
            {
                //PPB.GetSystem<ui_system>().TriggerSoundEntity("ButtonClickSFX");
                PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");
                PPB.TogglePause(true);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PLAYUI), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PAUSE), true);
                PPB.GetSystem<ui_system>().UpdateMaximumIndex();
                PPB.SetTabPaused(false);
            }
            else
            {

                //PPB.GetSystem<ui_system>().TriggerSoundEntity("ButtonClickSFX");
                PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");
                PPB.TogglePause(false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PAUSE), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::QUIT), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::RESTART), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::RETURNMAINMENU), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY1), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY2), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY3), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY4), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY5), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY6), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::HOWTOPLAY7), false);
                PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(UI_LAYER::PLAYUI), true);
                PPB.GetSystem<ui_system>().UpdateMaximumIndex();
            }
        }

        if (PPB.GetPauseBool())
        {
            PPB.GetSystem<ui_system>().EnableControllerUIMode();
        }
        else
        {
            PPB.GetSystem<ui_system>().EnableControllerUIMode(false);
        }
    }
};