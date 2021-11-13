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

	paperback::Vector3f		m_ptCenter;							// ----- Center point
	float				m_fRadius;							// ----- Radius of circle

};
// ----- End of header guard
#endif
