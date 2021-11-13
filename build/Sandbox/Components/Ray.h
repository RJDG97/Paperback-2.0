// ----- Start of header guard
#ifndef RAY_H
#define RAY_H

#include "Math/Math_includes.h"

struct Ray
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Ray"
	};

	paperback::Vector3f m_Start;
	paperback::Vector3f m_Direction;

	bool				m_Collided;

	// utilized in debugDraw
	Ray(
		paperback::Vector3f start = paperback::Vector3f{},
		paperback::Vector3f dir = paperback::Vector3f{},
		bool collide = false) :
		m_Start{start},
		m_Direction{dir},
		m_Collided{collide}
	{}

	// add a transform later for utility purposes

	//paperback::Vector3f CastRay(float t)
	//{
	//	return m_start + m_direction * t;
	//}
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