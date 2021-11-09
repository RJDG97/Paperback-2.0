#pragma once

struct cost
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Cost"
	};

	paperback::i32     m_Value;
};

namespace RR_Cost
{
    RTTR_REGISTRATION
    {
       rttr::registration::class_<cost>( cost::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Cost", &cost::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}