#pragma once

struct pure_collider_tag
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "Is Jus An Invis Collider Fren"
    };
};

namespace RR_PURECOLLIDER_TAG
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<pure_collider_tag>(pure_collider_tag::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}