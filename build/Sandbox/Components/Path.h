#pragma once
#include "Math/Math_includes.h"

struct path
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Path"
	};

	void AddPoint( int Index, paperback::Vector3f Point ) noexcept
	{
		m_Points.insert( m_Points.begin() + Index, Point );
	}

	// Pass paperback::component::entity::m_GlobalIndex
	void RemovePoint( int Index ) noexcept
	{
		m_Points.erase( m_Points.begin() + Index );
	}

	std::vector<paperback::Vector3f> m_Points;
	int m_ID;
};


namespace RR_Path
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<path>(path::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Points", &path::m_Points)
			.property("Path ID", &path::m_ID)(rttr::policy::prop::as_reference_wrapper);
	}
}