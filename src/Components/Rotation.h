#include "Math/Math_includes.h"

#pragma once
struct rotation
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Rotation"
	};

	paperback::Vector3f	 m_Value;							// -- Scale vector
};

namespace RR_ROTATION
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<rotation>(rotation::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Rotation", &rotation::m_Value)(rttr::policy::prop::as_reference_wrapper);
	}
}