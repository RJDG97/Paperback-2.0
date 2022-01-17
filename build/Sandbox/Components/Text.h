#pragma once

struct text
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Text"
	};

	std::string m_Font;
	std::string m_Text;

	paperback::Vector3f m_Color;
	float m_FontSize = 1;
};

namespace RR_Text
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<text>(text::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Font", &text::m_Font)(rttr::policy::prop::as_reference_wrapper)
			.property("Text", &text::m_Text)(rttr::policy::prop::as_reference_wrapper)
			.property("Color", &text::m_Color)(rttr::policy::prop::as_reference_wrapper)
			.property("FontSize", &text::m_FontSize)(rttr::policy::prop::as_reference_wrapper);
	}
}