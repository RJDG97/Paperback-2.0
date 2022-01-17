#pragma once

struct ui
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "UI"
	};

	std::string m_Texture;
};

namespace RR_UI
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<ui>(ui::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Texture", &ui::m_Texture)(rttr::policy::prop::as_reference_wrapper);
	}
}