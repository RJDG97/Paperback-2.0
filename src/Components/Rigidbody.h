#pragma once

struct rigidbody
{
	constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Rigidbody"
    };

	xcore::vector2		m_Velocity;
	float				m_Mass;
};

namespace RR_Rigidbody
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<rigidbody>(rigidbody::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("Velocity", &rigidbody::m_Velocity)
            .property("Mass", &rigidbody::m_Mass);
    }
}