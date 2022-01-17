#pragma once

struct entityscript
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "EntityScript"
    };

    std::vector<std::string> m_ScriptID;
};

namespace RR_EntityScript
{
    RTTR_REGISTRATION
    {
         rttr::registration::class_<entityscript>(entityscript::typedef_v.m_pName)
           .constructor()(rttr::policy::ctor::as_object)
           .property("Script ID", &entityscript::m_ScriptID)(rttr::policy::prop::as_reference_wrapper);
    }
}