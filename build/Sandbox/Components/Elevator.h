#pragma once

struct elevator
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Elevator"
	};

	float m_StartTime{ -1 };
	float m_StopTime{ -1 };
	bool m_Activated;
};


namespace RR_Elevator
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<elevator>(elevator::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Start Time", &elevator::m_StartTime)(rttr::policy::prop::as_reference_wrapper)
			.property("Stop Time", &elevator::m_StopTime)(rttr::policy::prop::as_reference_wrapper)
			.property("Activated", &elevator::m_Activated);
	}
}