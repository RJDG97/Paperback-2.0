#pragma once

struct currency
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Currency"
	};

	paperback::i32     m_Value;
};

namespace RR_Currency
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<currency>( currency::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Currency", &currency::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}