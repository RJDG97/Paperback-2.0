#pragma once

struct mainmenu_mode_script : paperback::script::button_interface // Inherited Type (1)
{

    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Main Menu Mode Script"
    };

    // You have to define this constructor - For internal registration
    mainmenu_mode_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    void OnHover( void ) noexcept
    {
        PPB.ToggleCursor(true);
        PPB.GetSystem<ui_system>().EnableControllerUIMode();
    }

    void Run( void ) noexcept
    {
        PPB.ToggleCursor(true);
        PPB.GetSystem<ui_system>().EnableControllerUIMode();

        if (PPB.GetSystem<ui_system>().m_MaximumButtonIndex == 0)
            PPB.GetSystem<ui_system>().UpdateMaximumIndex();
    }
};