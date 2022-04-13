/**********************************************************************************
*\file         Listener.h
*\brief        Listener.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct listener
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Listener"
    };
};

namespace RR_Listener
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<listener>(listener::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}