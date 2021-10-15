#pragma once
#include "Editor/EditorInclude.h"
#include "Systems/WindowSystem.h"
#include "Json/paperback_json.h"
#include <rttr/type>

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