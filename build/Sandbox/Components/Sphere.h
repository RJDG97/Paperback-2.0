/**********************************************************************************
*\file         Sphere.h
*\brief        Sphere.h
*
*\author       Lee Jianyan, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Math_includes.h"

struct sphere
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Sphere"
	};

	float				m_Radius;							// ----- Radius of circle
	bool				m_Collided;							// ----- collided
};


namespace RR_SPHERE
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<sphere>(sphere::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Radius", &sphere::m_Radius)(rttr::policy::prop::as_reference_wrapper)
		   .property("Is Collide", &sphere::m_Collided);
	}
}

#endif
