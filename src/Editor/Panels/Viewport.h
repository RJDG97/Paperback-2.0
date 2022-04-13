/**********************************************************************************
*\file         Viewport.h
*\brief        Viewport.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Editor/EditorInclude.h"
#include <dearImGui/ImGuizmo/ImGuizmo.h>
#include "Functionality/Renderer/Renderer.h"
#include "Functionality/Camera/Camera.h"
//#include "paperback_camera.h"
#include "Editor/EditorMath.h"
#include "../../build/Sandbox/Systems/DebugSystem.h"
#include "../../build/Sandbox/Systems/RendererSystem.h"
#include "../../Mono.h"


struct EditorViewport : paperback::editor::instance
{
    int m_GizmoType = 7; //Set default as transform gizmo
    Camera3D m_Camera;

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