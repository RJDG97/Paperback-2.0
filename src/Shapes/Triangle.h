// ----- Start of header guard
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Math/Vector3f.h"
#include "LineSegment.h"

template <size_t N = 3>
struct Triangle
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Triangle"
	};

private:													// ----- Private members
	LineSegment m_arrSegments[N];
};
// -- To do:
//		crossProduct -> normal
//		center of triangle
//		vector to cover the triangle
//		for rotational physics, have a matrix to set all 3 points when in rotation

// for the normal, left hand, -> thumb x center finger = index finger
//   or               (negative)  -center finger x thumb = index finger

// ----- End of header guard
#endif