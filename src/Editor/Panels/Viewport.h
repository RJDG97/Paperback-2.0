#pragma once
#include "Editor/EditorInclude.h"
#include "Functionality/Renderer/Renderer.h"

struct EditorViewport : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Viewport"
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