#pragma once
#ifndef OFFSET_H
#define OFFSET_H

#include "Math/Vector3f.h"

struct offset
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Offset"
	};

	paperback::Vector3f	 m_Value;
};

namespace RR_Offset
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<offset>( offset::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Offset", &offset::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}

#endif