/**********************************************************************************
*\file         SpawnerDetails.h
*\brief        SpawnerDetails.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct spawner_details
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Spawner Details"
	};

	paperback::u32 m_GID;
};

namespace RR_SpawnerDetails
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<spawner_details>(spawner_details::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
			.property("Prefab GID", &spawner_details::m_GID);
	}
}