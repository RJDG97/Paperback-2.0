#pragma once

struct crosshair
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "Crosshair"
    };
};

namespace RR_CROSSHAIR
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<crosshair>(crosshair::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}