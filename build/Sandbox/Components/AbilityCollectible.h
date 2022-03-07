#pragma once

BEGIN_CREATE_DATA_COMPONENT( ability_collectible, Ability Collectible )
	
	bool m_Grow   = false;
	bool m_Shrink = false;
	bool m_Freeze = false;

END_CREATE_DATA_COMPONENT


namespace RR_AbilityCollectible
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_< ability_collectible >( ability_collectible::typedef_v.m_pName )
			.constructor()(rttr::policy::ctor::as_object)
			.property( "Enable Grow Ability?",   &ability_collectible::m_Grow   )( rttr::policy::prop::as_reference_wrapper )
			.property( "Enable Shrink Ability?", &ability_collectible::m_Shrink )( rttr::policy::prop::as_reference_wrapper )
			.property( "Enable Freeze Ability?", &ability_collectible::m_Freeze )( rttr::policy::prop::as_reference_wrapper );
	}
}