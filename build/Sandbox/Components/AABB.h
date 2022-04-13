/**********************************************************************************
*\file         AABB.h
*\brief        AABB.h
*
*\author       Lee Jianyan, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#ifndef AABB_H
#define AABB_H

#include "Math/Vector3f.h"

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