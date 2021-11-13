#pragma once

struct waypoint_tag
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "WaypointTag"
	};

	paperback::i32 m_Value;
};

namespace RR_WaypointTag
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<waypoint_tag>( waypoint_tag::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Tag", &waypoint_tag::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}