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

	rttr::registration::class_<xcore::math::vector3d>("Vector3d")
		.constructor()(rttr::policy::ctor::as_object)
		.property("x", &xcore::math::vector3d::m_X)
		.property("y", &xcore::math::vector3d::m_Y)
		.property("z", &xcore::math::vector3d::m_Z);

	rttr::registration::class_<xcore::math::vector3>("Vector3")
		.constructor()(rttr::policy::ctor::as_object)
		.property("x", &xcore::math::vector3::m_X)
		.property("y", &xcore::math::vector3::m_Y)
		.property("z", &xcore::math::vector3::m_Z)
		.property("w", &xcore::math::vector3::m_W);
	}
}