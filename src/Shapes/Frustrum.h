// ----- Start of header guard
#ifndef FRUSTRUM_H
#define FRUSTRUM_H

#include "Plane.h"

struct Frustum
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Frustum"
	};

	// The normals all point inwards (towards the centroid of the frustum)
	Plane m_planes[6];

	// To easily draw the frustum the original points it was constructed from are stored
	paperback::Vector3f m_points[8];

	Frustum()
	{
		// this plane must be in a box for it to work, with all its sides and surfaces
		// if cZero does not work, use paperback::Vector4f{}
		m_planes[0] = m_planes[1] = m_planes[2]
			= m_planes[3] = m_planes[4] = m_planes[5] = paperback::Vector4f{};
			
		m_points[0] = m_points[1] = m_points[2] = m_points[3] =
			m_points[4] = m_points[5] = m_points[6] = m_points[7] =
			paperback::Vector3f{};
	}

	void Set(const paperback::Vector3f& left_bot_near, const paperback::Vector3f& right_bot_near, 
		const paperback::Vector3f& right_top_near, const paperback::Vector3f& left_top_near,
		const paperback::Vector3f& left_bot_far, const paperback::Vector3f& right_bot_far, 
		const paperback::Vector3f& right_top_far, const paperback::Vector3f& left_top_far)
	{
		m_points[0] = left_bot_near;
		m_points[1] = right_bot_near;
		m_points[2] = right_top_near;
		m_points[3] = left_top_near;
		m_points[4] = left_bot_far;
		m_points[5] = right_bot_far;
		m_points[6] = right_top_far;
		m_points[7] = left_top_far;

		//left
		m_planes[0].Set(left_bot_far, left_top_far, left_bot_near);
		//right
		m_planes[1].Set(right_bot_near, right_top_far, right_bot_far);
		//top
		m_planes[2].Set(left_top_near, left_top_far, right_top_near);
		//bot
		m_planes[3].Set(right_bot_near, left_bot_far, left_bot_near);
		//near
		m_planes[4].Set(left_bot_near, left_top_near, right_bot_near);
		//far
		m_planes[5].Set(right_bot_far, right_top_far, left_bot_far);
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