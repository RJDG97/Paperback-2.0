// ----- Start of header guard
#ifndef POLYGON_SHAPE_H
#define POLYGON_SHAPE_H

#include "Math/Vector3f.h"
#include "Triangle.h"

template <size_t N = 4>
struct Polygon_shape
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Polygon_shape"
	};

private:													// ----- Private members

	// do not use this file
	Triangle m_arrSegments[N];
};

// -- To do:
//		center of polygon shape
//		find the min/max points to cover the polygon shape for 1st check collision (sphere/aabb)
//		for rotational physics, have a matrix that can be thrown into the triangles when in rotation

// for the normal, left hand, -> thumb x center finger = index finger
//   or               (negative)  -center finger x thumb = index finger

// ----- End of header guard
#endif