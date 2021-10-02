#pragma once

struct transform
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Transform"
	};

	xcore::vector3	 m_Offset;
	xcore::vector3	 m_Position;
	xcore::vector3	 m_Rotation;
	xcore::vector3	 m_LocalScale;
};

namespace RR_Transform
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<transform>( transform::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Offset", &transform::m_Offset )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Position", &transform::m_Position )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Rotation", &transform::m_Rotation )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Scale", &transform::m_LocalScale )( rttr::policy::prop::as_reference_wrapper );
    }
}