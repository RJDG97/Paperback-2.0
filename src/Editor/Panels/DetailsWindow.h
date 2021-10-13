#pragma once
#include "Editor/EditorInclude.h"

struct DetailsWindow : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Entity Details"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = true;
    }

    void Update(void) noexcept
    {
        ImGui::Begin(DetailsWindow::typedef_v.m_pName);

        //Component Inspector + Component Editor
        ImGui::End();
    }

};