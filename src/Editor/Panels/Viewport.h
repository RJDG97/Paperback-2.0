#pragma once
#include "Editor/EditorInclude.h"
#include <dearImGui/ImGuizmo/ImGuizmo.h>
#include "Functionality/Renderer/Renderer.h"
#include "paperback_camera.h"
#include "Editor/EditorMath.h"

struct EditorViewport : paperback::editor::instance
{
    int m_GizmoType = -1;

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

    void ViewportMenuBar();

    void ComposeTransform();

};