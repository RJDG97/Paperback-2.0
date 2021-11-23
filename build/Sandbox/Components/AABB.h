#pragma once
#ifndef AABB_H
#define AABB_H

#include "Math/Vector3f.h"
#include "Shapes/LineSegment.h"

struct aabb
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "AABB"
	};

	paperback::Vector3f m_point[8];
	bool m_Collided;
};

#endif