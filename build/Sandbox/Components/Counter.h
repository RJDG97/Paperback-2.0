#pragma once

struct counter
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Counter"
	};

	paperback::i32     m_Value;
};

namespace RR_Counter
{
    RTTR_REGISTRATION
    {
       rttr::registration::class_<counter>( counter::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Counter", &counter::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}