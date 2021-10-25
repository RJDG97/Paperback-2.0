#pragma once

struct listener
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Listener"
    };
};

namespace RR_Listener
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<listener>(listener::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object);
    }
}