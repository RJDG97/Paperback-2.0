#pragma once
#include "Editor/EditorInclude.h"

struct EntityInspector : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Entity Inspector"
    };

    void OnSystemCreated( void ) noexcept
    {
        m_bEnabled = false;
    }

    void Update( void ) noexcept
    {
        ImGui::Begin(EntityInspector::typedef_v.m_pName);


        ImGui::End();
    }

};