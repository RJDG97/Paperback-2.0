#pragma once
#include "Editor/EditorInclude.h"

struct ArchetypeInspector : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Prefabs"
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

    void PrefabPanel();
    
    void MenuBar();

    void DisplayPrefabComponents(paperback::archetype::instance* Archetype, paperback::u32 i, prefab* Prefab);
};  