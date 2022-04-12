#pragma once

struct transition
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Transition"
    };

    enum TransitionState
    {
        INACTIVE = 0,
        ENTERING_LEVEL,
        EXITING_LEVEL
    };

    TransitionState m_State = INACTIVE; // 0 inactive, 1 entering a level, 2 exiting a level
};


namespace RR_TRANSITION
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<transition>(transition::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("State", &transition::m_State);
    }
}