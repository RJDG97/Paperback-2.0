#pragma once

struct bounding_volume
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "Bounding Volume"
    };
};

namespace RR_BOUNDINGVOLUME
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<bounding_volume>(bounding_volume::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}