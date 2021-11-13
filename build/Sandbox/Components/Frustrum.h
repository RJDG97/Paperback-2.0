// ----- Start of header guard
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"

struct Frustum
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Frustum"
	};

	// The normals point inwards to the centroid of the frustum
	//Plane m_planes[6];

	// To draw the frustum from the original points
	paperback::Vector3f 
		m_pointA, // [0]
		m_pointB, // [1]
		m_pointC, // [2]
		m_pointD, // [3]
		m_pointE, // [4]
		m_pointF, // [5]
		m_pointG, // [6]
		m_pointH; // [7]

	bool				m_Collided;

	// utilized in debugDraw
	Frustum( 
		paperback::Vector3f ptA = paperback::Vector3f{},
		paperback::Vector3f ptB = paperback::Vector3f{},
		paperback::Vector3f ptC = paperback::Vector3f{},
		paperback::Vector3f ptD = paperback::Vector3f{},
		paperback::Vector3f ptE = paperback::Vector3f{},
		paperback::Vector3f ptF = paperback::Vector3f{},
		paperback::Vector3f ptG = paperback::Vector3f{},
		paperback::Vector3f ptH = paperback::Vector3f{},
		bool collide = false) :
		m_pointA{ ptA },
		m_pointB{ ptB },
		m_pointC{ ptC },
		m_pointD{ ptD },
		m_pointE{ ptE },
		m_pointF{ ptF },
		m_pointG{ ptG },
		m_pointH{ ptH },
		m_Collided{ collide }
	{}
//	union
//	{
//#pragma warning(disable : 4201)
//		struct
//		{
//		    Plane m_planeA, m_planeB, m_planeC, 
//					m_planeD, m_planeE, m_planeF;
//		};
//#pragma warning(default : 4201)
//		Plane array[6];
//	};

//	union
//	{
//#pragma warning(disable : 4201)
//		struct
//		{
//		    paperback::Vector3f m_pointA, m_pointB, m_pointC, m_pointD
//							m_pointE, m_pointF, m_pointG, m_pointH;
//		};
//#pragma warning(default : 4201)
//		paperback::Vector3f array[8];
//	};

	void Set(const paperback::Vector3f& left_bot_near, const paperback::Vector3f& right_bot_near, 
		const paperback::Vector3f& right_top_near, const paperback::Vector3f& left_top_near,
		const paperback::Vector3f& left_bot_far, const paperback::Vector3f& right_bot_far, 
		const paperback::Vector3f& right_top_far, const paperback::Vector3f& left_top_far)
	{
		m_pointA = left_bot_near;
		m_pointB = right_bot_near;
		m_pointC = right_top_near;
		m_pointD = left_top_near;
		m_pointE = left_bot_far;
		m_pointF = right_bot_far;
		m_pointG = right_top_far;
		m_pointH = left_top_far;

		////left
		//m_planes[0].Set(left_bot_far, left_top_far, left_bot_near);
		////right
		//m_planes[1].Set(right_bot_near, right_top_far, right_bot_far);
		////top
		//m_planes[2].Set(left_top_near, left_top_far, right_top_near);
		////bot
		//m_planes[3].Set(right_bot_near, left_bot_far, left_bot_near);
		////near
		//m_planes[4].Set(left_bot_near, left_top_near, right_bot_near);
		////far
		//m_planes[5].Set(right_bot_far, right_top_far, left_bot_far);
	}
};

// ----- End of header guard
#endif