#pragma once

struct pushable
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Pushable"
    };

    uint32_t m_State{0};    //-1 is shrink, 0 is default, 1 is grow
};


namespace RR_PUSHABLE
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<pushable>(pushable::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("State", &pushable::m_State);
    }
}