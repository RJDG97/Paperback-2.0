// ----- Start of header guard
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Math/Vector4f.h"
#include "Math/Vector3f.h"
#include "Plane.h"

struct Frustum
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Frustum"
	};

	//LineSegment m_arrSegments[N];

	Plane mPlanes[6];

	paperback::Vector3f mPoints[8];

	Frustum()
	{
		// this plane must be in a box for it to work, with all its sides and surfaces
		// if cZero does not work, use paperback::Vector4f{0.f,0.f,0.f,0.f}
		mPlanes[0] = mPlanes[1] = mPlanes[2]
			= mPlanes[3] = mPlanes[4] = mPlanes[5] = paperback::Vector4f::cZero;
			
		mPoints[0] = mPoints[1] = mPoints[2] = mPoints[3] = 
			mPoints[4] = mPoints[5] = mPoints[6] = mPoints[7] =
			paperback::Vector3f{};
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