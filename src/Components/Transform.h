#pragma once
#include <rttr/registration>

struct transform
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "transform"
	};

	xcore::vector2	 m_Offset;
	xcore::vector2	 m_Position;
	xcore::vector2	 m_Rotation;
	xcore::vector2	 m_LocalScale;
};

namespace RR_Transform
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<transform>("transform")
       .property("Offset", &transform::m_Offset)
       .property("Position", &transform::m_Position)
       .property("Rotation", &transform::m_Rotation)
       .property("Scale", &transform::m_LocalScale);
    }
}