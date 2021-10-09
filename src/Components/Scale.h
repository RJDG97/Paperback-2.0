#pragma once

#include "Math/Math_includes.h"

struct scale
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Scale"
	};

	paperback::Vector3f	 m_Value;
};

namespace RR_SCALE
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<scale>(scale::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Scale", &scale::m_Value)(rttr::policy::prop::as_reference_wrapper);
	}
}