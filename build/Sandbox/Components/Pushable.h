#pragma once

struct pushable
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "Pushable"
    };
};


// Tag Component Creation Sample
/*
BEGIN_CREATE_TAG_COMPONENT( pushable )
END_CREATE_TAG_COMPONENT
*/


namespace RR_PUSHABLE
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<pushable>( pushable::typedef_v.m_pName )
            .constructor()(rttr::policy::ctor::as_object);
    }
}