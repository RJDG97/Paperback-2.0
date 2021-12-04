#pragma once
// ----- Start of header guard
#ifndef PLANE_H
#define PLANE_H

#include "Math/Math_includes.h"

struct Plane
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Plane"
	};

	// distance from origin computed as
	// m_data.w = Dot(origin - pointOnPlane, normal)

	paperback::Vector4f m_data;
	bool				m_Collided;

	// no appropriate default constructor error
	Plane()
		: 
		m_data{ paperback::Vector4f{} },
		m_Collided(false)
	{}

	// utilized in geometry.h
	Plane(paperback::Vector3f a, paperback::Vector3f b, paperback::Vector3f c)
	{
		Set(a, b, c);
	}

	// Set a plane from triangle
	void Set(const paperback::Vector3f& p0, 
		const paperback::Vector3f& p1, 
		const paperback::Vector3f& p2)
	{
		// Get plane vectors
		paperback::Vector3f v1 = p1 - p0;
		paperback::Vector3f v2 = p2 - p0;

		// Get plane normal, normalize the plane normal
		paperback::Vector3f n = v1.Cross(v2).Normalized();

		// Set plane data
		float D = p0.Dot(n);
		m_data = paperback::Vector4f(n.x, n.y, n.z, D);
	}

	// Set a plane from point and normal
	void Set(const paperback::Vector3f& normal, 
		const paperback::Vector3f& point)
	{
		// normalize the plane normal
		paperback::Vector3f n = normal.Normalized();

		// Set plane data
		float D = point.Dot(n);
		m_data = paperback::Vector4f(n.x, n.y, n.z, D);
	}

	// Get plane normal
	paperback::Vector3f GetNormal() const
	{
		return paperback::Vector3f(m_data.x, m_data.y, m_data.z);
	}
};

namespace RR_Plane
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<Plane>(Plane::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Plane Point", &Plane::m_data)(rttr::policy::prop::as_reference_wrapper)
			.property("Is Collide", &Plane::m_Collided)(rttr::policy::prop::as_reference_wrapper);
	}
}

// ----- End of header guard
#endif