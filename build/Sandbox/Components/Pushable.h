#pragma once

struct pushable
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "Pushable"
    };
};

namespace RR_PUSHABLE
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<pushable>( pushable::typedef_v.m_pName )
            .constructor()(rttr::policy::ctor::as_object);
    }
}