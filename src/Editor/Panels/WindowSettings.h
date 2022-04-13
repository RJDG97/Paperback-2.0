/**********************************************************************************
*\file         WindowSettings.h
*\brief        WindowSettings.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Editor/EditorInclude.h"
#include "WindowSystem.h"
#include "Json/paperback_json.h"
#include <rttr/type>
#include "Functionality/Renderer/Renderer.h"

struct WindowSettings : paperback::editor::instance
{
    paperback::JsonFile JFile;
    GLFWwindow* m_pWindow;

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Window Settings"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = false;
        m_pWindow = PPB.GetSystem<window_system>().m_pWindow;

    }

    void Update(void) noexcept
    {
        Panel();
    }

    void Panel();
};