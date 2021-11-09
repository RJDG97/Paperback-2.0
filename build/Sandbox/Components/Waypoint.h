#pragma once

#include "Math/Vector3f.h"
#include "Math/Mtx4x4.h"

struct waypoint
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Waypoint"
	};

	paperback::Vector3f	 m_Value;
};

namespace RR_Waypoint
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<waypoint>( waypoint::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Destination", &waypoint::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}