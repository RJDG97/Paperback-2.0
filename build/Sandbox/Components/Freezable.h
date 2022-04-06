#pragma once

struct freezable
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Freezable"
    };

    bool m_Frozen;
};


// Tag Component Creation Sample
/*
BEGIN_CREATE_TAG_COMPONENT( freezable )
END_CREATE_TAG_COMPONENT
*/


namespace RR_FREEZABLE
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<freezable>(freezable::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("Frozen", &freezable::m_Frozen);
    }
}