// ----- Start of header guard
#ifndef RAY_H
#define RAY_H

#include "Math/Vector3f.h"

//template <size_t N = 3>
struct Ray
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Ray"
	};

	//LineSegment m_arrSegments[N];

	paperback::Vector3f m_start;
	paperback::Vector3f m_direction;

	Ray()
	{
		m_start = m_direction = paperback::Vector3f{};
	}

	Ray(paperback::Vector3f start, paperback::Vector3f dir)
	{
		m_start = start;
		m_direction = dir;
	}

	// add a transform later for utility purposes

	paperback::Vector3f CastRay(float t)
	{
		return m_start + m_direction * t;
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