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