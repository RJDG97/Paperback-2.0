#pragma once

struct card
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Card"
	};

	paperback::u32     m_unitgid;
};

namespace RR_Card
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<card>(card::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
			.property("Unit", &card::m_unitgid)(rttr::policy::prop::as_reference_wrapper);
	}
}