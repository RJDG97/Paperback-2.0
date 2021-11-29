#pragma once

struct path_follower
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Path Follower"
	};

	int m_ID;
	float m_Distance;
	float m_TravelSpeed;
	paperback::Vector3f m_Direction;
};


namespace RR_PathFollower
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<path_follower>(path_follower::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Path ID", &path_follower::m_ID)(rttr::policy::prop::as_reference_wrapper)
			.property("Distance", &path_follower::m_Distance)(rttr::policy::prop::as_reference_wrapper)
			.property("Travel Speed", &path_follower::m_TravelSpeed)(rttr::policy::prop::as_reference_wrapper);
	}
}