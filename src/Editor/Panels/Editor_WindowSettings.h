#pragma once
#include "Editor/EditorInclude.h"

struct WindowSettings : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Amend Window Details"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = false;
        
    }

    void Update(void) noexcept
    {
        ImGui::Begin(WindowSettings::typedef_v.m_pName);


        ImGui::End();
    }

};