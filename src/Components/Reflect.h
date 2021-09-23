#pragma once

namespace paperback
{
	// register the math libs here
	RTTR_REGISTRATION
	{
		rttr::registration::class_<xcore::math::vector2>("Vector2")
		.constructor()(rttr::policy::ctor::as_object)
		.property("x", &xcore::math::vector2::m_X)
		.property("y", &xcore::math::vector2::m_Y);
	}
}