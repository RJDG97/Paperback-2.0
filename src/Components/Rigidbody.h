#pragma once
#include <rttr/registration>

struct rigidbody
{
	constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "rigidbody"
    };

	xcore::vector2		m_Velocity;
	float				m_Mass;
};

namespace RR_Rigidbody
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<rigidbody>("rigidbody")
            .property("Velocity", &rigidbody::m_Velocity)
            .property("Mass", &rigidbody::m_Mass);
    }
}