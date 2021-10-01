// ----- Start of header guard
#ifndef AABB_H
#define AABB_H

#include "Math/Vector3f.h"
#include "LineSegment.h"

struct AABB
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "AABB"
	};

public:													// ----- Private members
	paperback::Vector3f m_point[8];


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