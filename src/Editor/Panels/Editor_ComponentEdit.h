#pragma once
#include "Editor/EditorInclude.h"

struct ComponentEditor : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Add/Remove Component(s)"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = false;
    }

    void Update(void) noexcept
    {
        ImGui::Begin(ComponentEditor::typedef_v.m_pName);


        ImGui::End();
    }

};