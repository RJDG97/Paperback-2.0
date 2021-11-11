// ----- Start of header guard
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Math/Vector3f.h"

struct Triangle
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Triangle"
	};

	paperback::Vector3f m_points[3];

	Triangle()
	{
		m_points[0] = m_points[1] = m_points[2] = paperback::Vector3f{};
	}

	Triangle(const paperback::Vector3f& p0, 
		const paperback::Vector3f& p1, 
		const paperback::Vector3f& p2)
	{
		m_points[0] = p0;
		m_points[1] = p1;
		m_points[2] = p2;
	}
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