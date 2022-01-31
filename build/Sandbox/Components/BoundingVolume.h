#pragma once

struct bounding_volume
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "Bounding Volume"
    };
};

namespace RR_BASE
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<base>(base::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}