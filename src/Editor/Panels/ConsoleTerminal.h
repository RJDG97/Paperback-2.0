/**********************************************************************************
*\file         ConsoleTerminal.h
*\brief        ConsoleTerminal.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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