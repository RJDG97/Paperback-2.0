#pragma once

struct health
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Health"
	};

	paperback::i32     m_CurrentHealth;
	paperback::i32     m_MaxHealth;
};

namespace RR_Health
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<health>( health::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Current Health", &health::m_CurrentHealth )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Max Health", &health::m_MaxHealth )( rttr::policy::prop::as_reference_wrapper );
    }
}