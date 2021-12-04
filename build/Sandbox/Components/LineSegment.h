#pragma once
// ----- Start of header guard
#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Math/Math_includes.h"

// -- Important Note: All line segments must follow and close the polygon shape in a anti-clockwise manner
struct LineSegment
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "LineSegment"
	};

	paperback::Vector3f m_Start;			// ----- Start pt of segment
	paperback::Vector3f m_End;				// ----- End pt of segment
	bool				m_Collided;

	// utilized in DebugSystem.h
	LineSegment(
		paperback::Vector3f start = paperback::Vector3f{},
		paperback::Vector3f end = paperback::Vector3f{},
		bool collide = false)
		:
		m_Start{ start },
		m_End{end},
		m_Collided{collide}
	{
	}
};

namespace RR_LineSegment
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<LineSegment>(LineSegment::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Start Point", &LineSegment::m_Start)(rttr::policy::prop::as_reference_wrapper)
			.property("End Point", &LineSegment::m_End)(rttr::policy::prop::as_reference_wrapper)
			.property("Is Collide", &LineSegment::m_Collided)(rttr::policy::prop::as_reference_wrapper);
	}
}

// ----- End of header guard
#endif