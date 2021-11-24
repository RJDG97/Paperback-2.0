#pragma once

struct healthbar
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "HealthBar"
    };
};

namespace RR_HEALTHBAR
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<healthbar>(healthbar::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}