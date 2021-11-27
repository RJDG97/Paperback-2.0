#pragma once

struct deck
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Deck"
	};

	std::array<std::pair<paperback::u32, int>, 3> m_Deck;
};

namespace RR_Deck
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<deck>(deck::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object);
	}
}