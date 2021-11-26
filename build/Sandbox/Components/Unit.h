#pragma once

struct unit
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "Unit"
    };
};

namespace RR_UNIT
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<unit>(unit::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}