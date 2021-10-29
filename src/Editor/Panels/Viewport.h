#pragma once
#include "Editor/EditorInclude.h"
#include <dearImGui/ImGuizmo/ImGuizmo.h>
#include "Functionality/Renderer/Renderer.h"
#include "paperback_camera.h"
#include <rttr/type>

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

    void Gizmo();

};