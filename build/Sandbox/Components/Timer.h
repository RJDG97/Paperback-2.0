/**********************************************************************************
*\file         Timer.h
*\brief        Timer.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct timer
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Timer"
    };

	float m_Cooldown = 0.0f;
	float m_Value = 0.0f;
};

namespace RR_Timer
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<timer>(timer::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property( "Cooldown Timer", &timer::m_Cooldown )
            .property( "Timer", &timer::m_Value );
    }
}