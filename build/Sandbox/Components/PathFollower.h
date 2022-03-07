#pragma once

struct path_follower
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Path Follower"
	};

	int m_PathID{};
	float m_Distance{0.0f};
	float m_TravelSpeed{};;
	bool m_BackAndForth{};
	bool m_Reversed{};
	bool m_PauseTravel{};

	paperback::Vector3f m_Direction{};
	bool m_FinishedTravelling{};
};


namespace RR_PathFollower
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<path_follower>(path_follower::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Path ID", &path_follower::m_PathID)(rttr::policy::prop::as_reference_wrapper)
			.property("Distance", &path_follower::m_Distance)(rttr::policy::prop::as_reference_wrapper)
			.property("Travel Speed", &path_follower::m_TravelSpeed)(rttr::policy::prop::as_reference_wrapper)
			.property("Back and Forth", &path_follower::m_BackAndForth)(rttr::policy::prop::as_reference_wrapper)
			.property("Reversed", &path_follower::m_Reversed)(rttr::policy::prop::as_reference_wrapper)
			.property("Pause Travel", &path_follower::m_PauseTravel)(rttr::policy::prop::as_reference_wrapper);
	}
}