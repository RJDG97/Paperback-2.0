// ----- Start of header guard
#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Math/Vector3f.h"

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

// for the normal, left hand, -> thumb x center finger = index finger
//   or               (negative)  -center finger x thumb = index finger

// ----- End of header guard
#endif