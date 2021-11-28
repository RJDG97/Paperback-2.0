#pragma once

enum class CardState : int
{
    DEFAULT = 0
,   HOVERED
,   CLICKED
};

struct card
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Card"
	};

	void SetCardState( const CardState State ) noexcept
	{
		m_CardState = static_cast<int>( State );
	}

	bool IsCardState( const CardState State ) const noexcept
	{
		return State == static_cast<CardState>( m_CardState );
	}

	std::array<std::string, 3>     m_CardStateTextures{ };                                    // Stores different state textures - Default, Hovered, Clicked
	paperback::Vector3f            m_OriginalPosition{ };									  // Original Position Before Drag & Release - Reset to this Position if not placed on lane
	paperback::u64                 m_ReferencedScript = 0;                                    // Guid of referenced script
	paperback::u32                 m_UnitGID;
	int                            m_CardState = static_cast<int>( CardState::DEFAULT );      // Current card state
	int				               m_PositionIndex;
};

namespace RR_Card
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<card>( card::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
			.property( "Card Textures", &card::m_CardStateTextures)(rttr::policy::prop::as_reference_wrapper )
			.property( "Original Position", &card::m_OriginalPosition )
			.property( "Script Guid", &card::m_ReferencedScript )
			.property( "Unit GID", &card::m_UnitGID )(rttr::policy::prop::as_reference_wrapper )
			.property( "Card State", &card::m_CardState )
			.property( "Position Index", &card::m_PositionIndex )(rttr::policy::prop::as_reference_wrapper );
	}
}