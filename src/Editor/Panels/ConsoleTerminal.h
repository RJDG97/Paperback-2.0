#pragma once
#include "Editor/EditorInclude.h"
#include <dearImGui/imterm/terminal.hpp>

struct ConsoleTerminal : paperback::editor::instance
{
    bool showing_term = true;

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Console"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = true;
    }

    void Update(void) noexcept
    {
        Panel();
    }

    void Panel();
};