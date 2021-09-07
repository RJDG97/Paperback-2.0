#pragma once

struct rigidbody
{
	constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "rigidbody"
    };

	xcore::vector2		m_Velocity;
	float				m_Mass;
};