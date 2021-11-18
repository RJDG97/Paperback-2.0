#pragma once

#include "Math/Vector3f.h"

struct rigidbody
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Rigidbody"
    };

    paperback::Vector3f		m_Accel;
    paperback::Vector3f		m_Velocity;
};

namespace RR_Rigidbody
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<rigidbody>(rigidbody::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("Acceleration", &rigidbody::m_Accel)(rttr::policy::prop::as_reference_wrapper)
            .property("Velocity", &rigidbody::m_Velocity)(rttr::policy::prop::as_reference_wrapper);
    }
}