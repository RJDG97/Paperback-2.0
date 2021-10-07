#pragma once

#include "Math/Vector3f.h"
#include "Math/Mtx4x4.h"

struct transform
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Transform"
	};

	xcore::vector3	 m_Offset;
	xcore::vector3	 m_Position;							// -- Position vector

	//paperback::Mtx4x4 m_mtxTrans;								// -- The translatation matrix
	//paperback::Mtx4x4 m_mtxRot;									// -- The rotation matrix
	//paperback::Mtx4x4 m_mtxScale;								// -- The scale matrix
	//static paperback::Mtx4x4 m_mtxConversion;					// -- Matrix that converts coordinates from NDC to actual
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