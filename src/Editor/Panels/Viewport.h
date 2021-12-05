#pragma once
#include "Editor/EditorInclude.h"
#include <dearImGui/ImGuizmo/ImGuizmo.h>
#include "Functionality/Renderer/Renderer.h"
#include "paperback_camera.h"
#include "Editor/EditorMath.h"
#include "../../build/Sandbox/Systems/DebugSystem.h"

struct EditorViewport : paperback::editor::instance
{
    int m_GizmoType = 7; //Set default as transform gizmo

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

    void ViewportOne();
    void ViewportTwo();

    void Gizmo();

    void MouseSelection();

    void ViewportMenuBar();

    void ComposeTransform();

};