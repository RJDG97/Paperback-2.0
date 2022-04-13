/**********************************************************************************
*\file         CardScript.h
*\brief        CardScript.h
*
*\author       Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct card_script : paperback::script::card_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Card Script"
    };

    // You have to define this constructor - For internal registration
    card_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::card_interface{ Instance }
    { }

    // Override virtual functions defined within button_interface
    void OnHover(void) noexcept override
    {
    }

    void OnClick(paperback::component::entity& Entity, paperback::u32 PrefabGID, int PositionIndex ) noexcept override
    {
        // Removed - Unused
    }
};