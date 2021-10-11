#pragma once
#include "Editor/EditorInclude.h"

struct ComponentInspector : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Entity Details"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = false;
    }

    void Update(void) noexcept
    {
        ImGui::Begin(ComponentInspector::typedef_v.m_pName);


        ImGui::End();
    }

};