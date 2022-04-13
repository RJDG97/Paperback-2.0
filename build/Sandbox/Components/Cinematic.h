/**********************************************************************************
*\file         Cinematic.h
*\brief        Cinematic.h
*
*\author       Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct cinematic
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Cinematic"
    };

    struct CinematicInfo
    {
        CinematicInfo()
            : m_MoveSpeed{},
              m_HoldTime{}
        {

        }

        float m_MoveSpeed;
        float m_HoldTime;
    };

    void AddPoint(int Index) noexcept
    {
        m_CinematicInfos.insert(m_CinematicInfos.begin() + Index, CinematicInfo());
    }

    void RemovePoint(int Index) noexcept
    {
        m_CinematicInfos.erase(m_CinematicInfos.begin() + Index);
    }
    
    std::vector<CinematicInfo> m_CinematicInfos;
    bool m_OnHold{false};
    int m_Index{0};
    float m_Timer{0.0f};
};

namespace RR_Cinematic
{
    RTTR_REGISTRATION
    {
         rttr::registration::class_<cinematic>(cinematic::typedef_v.m_pName)
           .constructor()(rttr::policy::ctor::as_object)
           .property("Cinematic Infos", &cinematic::m_CinematicInfos)
           .property("Index", &cinematic::m_Index)
           .property("On Hold", &cinematic::m_OnHold)
           .property("Timer", &cinematic::m_Timer);
    }
}