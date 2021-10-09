// ----- Start of header guard
#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Vector3f.h"
#include "Math/MathUtils.h"

struct Sphere
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Sphere"
	};

	float				m_fRadius;							// ----- Radius of circle
	bool				m_Collided;							// ----- collided
};

//		this will mainly be used as a 1st/2nd step collision check

// ----- End of header guard


namespace RR_SPHERE
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<Sphere>(Sphere::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Radius", &Sphere::m_fRadius)(rttr::policy::prop::as_reference_wrapper)
		   .property("Is Collide", &Sphere::m_Collided);
	}
}

#endif
