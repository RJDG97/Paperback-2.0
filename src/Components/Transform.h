#pragma once

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