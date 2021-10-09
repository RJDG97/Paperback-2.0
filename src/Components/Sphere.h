#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Vector3f.h"
#include "Math/MathUtils.h"

struct sphere
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Sphere"
	};

	float				m_fRadius;							// ----- Radius of circle
	bool				m_Collided;							// ----- collided
};


namespace RR_SPHERE
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<sphere>(sphere::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Radius", &sphere::m_fRadius)(rttr::policy::prop::as_reference_wrapper)
		   .property("Is Collide", &sphere::m_Collided);
	}
}

#endif
