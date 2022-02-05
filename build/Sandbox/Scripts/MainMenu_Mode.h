#pragma once

struct mainmenu_mode_script : paperback::script::button_interface // Inherited Type (1)
{
    bool m_Init;

    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Main Menu Mode Script"
    };

    // You have to define this constructor - For internal registration
    mainmenu_mode_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance },
        m_Init{}
    { }

    void OnHover( void ) noexcept
    {
        PPB.ToggleCursor(true);
    }
};