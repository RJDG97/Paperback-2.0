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

	paperback::Vector3f	 m_PosOffset;
	paperback::Vector3f	 m_RotOffset;
	paperback::Vector3f	 m_ScaleOffset;
};

namespace RR_Offset
{    
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<offset>(offset::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Postion Offset", &offset::m_PosOffset)(rttr::policy::prop::as_reference_wrapper)
		   .property("Rotation Offset", &offset::m_RotOffset)(rttr::policy::prop::as_reference_wrapper)
		   .property("Scale Offset", &offset::m_ScaleOffset)(rttr::policy::prop::as_reference_wrapper);
    }
}

#endif