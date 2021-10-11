#pragma once
#include "Editor/EditorInclude.h"

struct ArchetypeInspector : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Archetypes/Prefabs"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = false;
    }

    void Update(void) noexcept
    {
        ImGui::Begin(ArchetypeInspector::typedef_v.m_pName);


        ImGui::End();
    }

};