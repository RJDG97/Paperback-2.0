/**********************************************************************************
*\file         Slope.h
*\brief        Slope.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#ifndef SLOPE_H
#define SLOPE_H

#include "Math/Vector3f.h"

struct slope
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Slope"
	};

	bool m_North, m_South, m_East, m_West;	//bool values determining side of box that is sloping ONLY PICK ONE


	// slave component to bounding box, used to determine if an object is a slope 
	slope() :
		m_North{ false },
		m_South{ false },
		m_East{ false },
		m_West{ false }
	{}
};


namespace RR_SLOPE
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<slope>(slope::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("TOGGLE ONE ONLY: North", &slope::m_North)(rttr::policy::prop::as_reference_wrapper)
		   .property("South", &slope::m_South)(rttr::policy::prop::as_reference_wrapper)
		   .property("East", &slope::m_East)(rttr::policy::prop::as_reference_wrapper)
		   .property("West", &slope::m_West)(rttr::policy::prop::as_reference_wrapper);
	}
}
// ----- End of header guard
#endif