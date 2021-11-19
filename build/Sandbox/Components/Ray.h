#pragma once
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
	paperback::Vector3f m_Direction;	// -- Direction is the speed (not normalized*)

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

namespace RR_Ray
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<Ray>(Ray::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Start Point", &Ray::m_Start)(rttr::policy::prop::as_reference_wrapper)
			.property("Direction Normalised", &Ray::m_Direction)(rttr::policy::prop::as_reference_wrapper)
			.property("Is Collide", &Ray::m_Collided)(rttr::policy::prop::as_reference_wrapper);
	}
}

// ----- End of header guard
#endif