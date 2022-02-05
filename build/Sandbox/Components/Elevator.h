#pragma once

struct elevator
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Elevator"
	};

	int m_StartFrame{ -1 };
	int m_StopFrame{ -1 };
	bool m_Activated;
};


namespace RR_Elevator
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<elevator>(elevator::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Start Frame", &elevator::m_StartFrame)(rttr::policy::prop::as_reference_wrapper)
			.property("Stop Frame", &elevator::m_StopFrame)(rttr::policy::prop::as_reference_wrapper)
			.property("Activated", &elevator::m_Activated);
	}
}