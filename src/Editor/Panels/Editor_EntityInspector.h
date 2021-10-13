#pragma once
#include "Editor/EditorInclude.h"

struct imgui_system;

struct EntityInspector : paperback::editor::instance
{
    //imgui_system* m_pImgui;

    //paperback::system::instance* m_pImgui;

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "World Outliner"
    };

    void OnSystemCreated( void ) noexcept
    {
        //m_pImgui = PPB.GetSystem<imgui_system>();
        m_bEnabled = true;
    }

    void Update( void ) noexcept;

    void DeleteEntity( std::string WindowName, paperback::u32 EntityIndex, std::string EntityLabel );
};


