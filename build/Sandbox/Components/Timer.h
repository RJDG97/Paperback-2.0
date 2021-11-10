#pragma once

struct timer
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Timer"
    };

	float m_Value = 0.0f;
};

namespace RR_Timer
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<timer>(timer::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property( "Timer", &timer::m_Value )(rttr::policy::prop::as_reference_wrapper);
    }
}