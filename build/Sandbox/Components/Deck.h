#pragma once

struct CardDetails {
	std::uint32_t  m_CardGID;
	int				m_Count;
};

struct deck
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Deck"
	};

	std::array<CardDetails, 3> m_Deck;
	std::array<paperback::Vector3f, 3> m_Position;
};

namespace RR_Deck
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<deck>(deck::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
			.property("Deck Details", &deck::m_Deck)(rttr::policy::prop::as_reference_wrapper);
	}
}