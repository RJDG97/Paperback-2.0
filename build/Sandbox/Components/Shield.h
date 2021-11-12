#pragma once

struct shield
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Shield"
	};

	int     m_Value;
};

namespace RR_Shield
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<shield>( shield::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Shield", &shield::m_Value )/*( rttr::policy::prop::as_reference_wrapper )*/;
    }
}