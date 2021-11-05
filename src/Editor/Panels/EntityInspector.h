#pragma once
#include "Editor/EditorInclude.h"

struct EntityInspector : paperback::editor::instance
{
    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "World Outliner"
    };

    void OnSystemCreated( void ) noexcept
    {
        m_bEnabled = true;
    }

    void Update( void ) noexcept
    {
        Panel();
    }

    void Panel();

    void DisplayEntities();

    void DisplayChildEntities( parent& Parent ); //Recursively Display

    void DeleteEntity( std::string WindowName, paperback::u32 EntityIndex );
};


