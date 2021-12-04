#pragma once
// ----- Start of header guard
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Math/Math_includes.h"

struct Triangle
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Triangle"
	};

	paperback::Vector3f m_pointA; //[0]
	paperback::Vector3f m_pointB; //[1]
	paperback::Vector3f m_pointC; //[2]

	bool				m_Collided;

	// utilized in debugDraw
	Triangle(
		paperback::Vector3f ptA = paperback::Vector3f{},
		paperback::Vector3f ptB = paperback::Vector3f{}, 
		paperback::Vector3f ptC = paperback::Vector3f{}, 
		bool collide = false) :
		m_pointA{ptA},
		m_pointB{ptB},
		m_pointC{ptC},
		m_Collided{collide}
	{}

//	union
//    {
//#pragma warning(disable : 4201)
//        struct
//        {
//            paperback::Vector3f m_pointA, m_pointB, m_pointC;
//        };
//#pragma warning(default : 4201)
//        paperback::Vector3f array[3];
//    };
};

namespace RR_Triangle
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<Triangle>(Triangle::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Point A", &Triangle::m_pointA)(rttr::policy::prop::as_reference_wrapper)
			.property("Point B", &Triangle::m_pointB)(rttr::policy::prop::as_reference_wrapper)
			.property("Point C", &Triangle::m_pointC)(rttr::policy::prop::as_reference_wrapper)
			.property("Is Collide", &Triangle::m_Collided)(rttr::policy::prop::as_reference_wrapper);
	}
}

// ----- End of header guard
#endif