/**********************************************************************************
*\file         EntityScript.h
*\brief        EntityScript.h
*
*\author       Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct entityscript
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "EntityScript"
    };

    struct ScriptID
    {
        std::string m_ID;
    };

    std::vector<ScriptID> m_ScriptID;
};

namespace RR_EntityScript
{
    RTTR_REGISTRATION
    {
         rttr::registration::class_<entityscript>(entityscript::typedef_v.m_pName)
           .constructor()(rttr::policy::ctor::as_object)
           .property("Script ID", &entityscript::m_ScriptID);
    }
}