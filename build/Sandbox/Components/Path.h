#pragma once
#include "Math/Math_includes.h"

struct path
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Path"
	};

	paperback::Spline m_Spline;
	int m_ID;
};


namespace RR_Path
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<path>(path::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Spline", &path::m_Spline)
			.property("Path ID", &path::m_ID);
	}
}