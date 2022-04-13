/**********************************************************************************
*\file         EntityInspector.h
*\brief        EntityInspector.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

    void DeleteEntity( std::string WindowName, paperback::u32 EntityIndex );
};


