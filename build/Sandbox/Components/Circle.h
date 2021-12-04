#pragma once
// ----- Start of header guard
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Math/Vector3f.h"

struct Circle
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Circle"
	};
	// excluded
	paperback::Vector3f		m_Center;							// ----- Center point
	float					m_Radius;							// ----- Radius of circle
};

// ----- End of header guard
#endif
