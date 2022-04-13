/**********************************************************************************
*\file         Selected.h
*\brief        Selected.h
*
*\author       Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct selected
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Selected"
    };

    bool m_Value = false;
};

namespace RR_Selected
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<selected>( selected::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Selected", &selected::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}