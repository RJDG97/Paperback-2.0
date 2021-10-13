#pragma once
#include "Editor/EditorInclude.h"

struct AssetBrowser : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Asset Browser"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = true;
    }

    void Update(void) noexcept
    {
        ImGui::Begin(AssetBrowser::typedef_v.m_pName);


        ImGui::End();
    }

};