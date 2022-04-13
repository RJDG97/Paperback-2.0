/**********************************************************************************
*\file         Spawner.h
*\brief        Spawner.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct spawner
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Spawner"
	};

	std::array<paperback::Vector3f, 3> m_Position;
	int lane;
};

namespace RR_Spawner
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<spawner>(spawner::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
			.property("Spawn Position", &spawner::m_Position);
	}
}