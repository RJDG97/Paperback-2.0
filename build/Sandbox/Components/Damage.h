#pragma once

struct damage
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Damage"
	};

	paperback::u32     m_Value;
};

namespace RR_Damage
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<damage>( damage::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Damage", &damage::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}