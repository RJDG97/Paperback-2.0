/**********************************************************************************
*\file         Player.h
*\brief        Player.h
*
*\author       Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct player
{
	constexpr static auto typedef_v = paperback::component::type::tag
	{
		.m_pName = "Player"
	};
};

namespace RR_Player
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<player>( player::typedef_v.m_pName )
            .constructor()( rttr::policy::ctor::as_object );
    }
}