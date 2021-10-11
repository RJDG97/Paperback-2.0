#pragma once
#include "Editor/EditorPanels.h"
#include "Editor/EditorPanels_inline.h"
#include "dearImGui/imgui.h"

struct EntityInspector : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "EntityInspector"
    };

    void OnSystemCreated( void ) noexcept
    {

    }

    void Update( void ) noexcept
    {
        ImGui::Begin(EntityInspector::typedef_v.m_pName);

        ImGui::Text("SUCCESS");

        ImGui::End();
    }

};