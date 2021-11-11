// ----- Start of header guard
#ifndef PLANE_H
#define PLANE_H

#include "Math/Vector3f.h"
#include "Math/Vector4f.h"

struct Plane
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Plane"
	};

	paperback::Vector4f m_data;

	Plane()
	{
		m_data = paperback::Vector4f{};
	}

	// Create the plane from a triangle
	Plane(const paperback::Vector3f& p0, 
		const paperback::Vector3f& p1, 
		const paperback::Vector3f& p2)
	{
		Set(p0, p1, p2);
	}

	// Create the plane from a point and a normal
	Plane(const paperback::Vector3f& normal, 
		const paperback::Vector3f& point)
	{
		Set(normal, point);
	}

	// Create a plane from a triangle. The plane's normal should be normalized.
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

	// Create a plane from a point and a normal. The plane's normal should be normalized.
	void Set(const paperback::Vector3f& normal, 
		const paperback::Vector3f& point)
	{
		// normalize the plane normal
		paperback::Vector3f n = normal.Normalized();

		// Set plane data
		float D = point.Dot(n);
		m_data = paperback::Vector4f(n.x, n.y, n.z, D);
	}

	// Get the normal of this plane
	paperback::Vector3f GetNormal() const
	{
		return paperback::Vector3f(m_data.x, m_data.y, m_data.z);
	}

	// Get the 'd' value (the distance from the origin) 
	// which can be computed as Dot(origin - pointOnPlane, normal)
	float GetDistance() const
	{
		return m_data.w;
	}
};

// ----- End of header guard
#endif