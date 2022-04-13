/**********************************************************************************
*\file         Transform.h
*\brief        Transform.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "Math/Vector3f.h"
#include "Math/Mtx4x4.h"

struct transform
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Transform"
	};

	paperback::Vector3f	 m_Offset;
	paperback::Vector3f	 m_Position;
};

namespace RR_Transform
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<transform>( transform::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Offset", &transform::m_Offset )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Position", &transform::m_Position )( rttr::policy::prop::as_reference_wrapper );
    }
}