#pragma once

struct icon
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "Icon"
    };
};

namespace RR_ICON
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<icon>(icon::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}