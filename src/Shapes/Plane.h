// ----- Start of header guard
#ifndef PLANE_H
#define PLANE_H

#include "Math/Vector3f.h"
#include "Math/Vector4f.h"
#include "LineSegment.h"

struct Plane
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Plane"
	};

public:													// ----- Private members
	Plane()
	{
		mData = paperback::Vector4f::cZero;
	}
	// Create the plane from a triangle
	Plane(const paperback::Vector3f& p0, const paperback::Vector3f& p1, const paperback::Vector3f& p2)
	{
		Set(p0, p1, p2);
	}
	// Create the plane from a point and a normal
	Plane(const paperback::Vector3f& normal, const paperback::Vector3f& point)
	{
		Set(normal, point);
	}

	// Create a plane from a triangle. The plane's normal should be normalized.
	void Set(const paperback::Vector3f& p0, const paperback::Vector3f& p1, const paperback::Vector3f& p2)
	{
		// Set mData from the 3 points. Note: You should most likely normalize the plane normal.
		paperback::Vector3f v1 = p1 - p0;
		paperback::Vector3f v2 = p2 - p0;
		paperback::Vector3f n = v1.Cross(v2).Normalized();
		float D = p0.Dot(n);
		mData = paperback::Vector4f(n.x, n.y, n.z, D);
	}
	// Create a plane from a point and a normal. The plane's normal should be normalized.
	void Set(const paperback::Vector3f& normal, const paperback::Vector3f& point)
	{
		// Set mData from the normal and point. Note: You should most likely normalize the plane normal.
		paperback::Vector3f n = normal.Normalized();
		float D = point.Dot(n);
		mData = paperback::Vector4f(n.x, n.y, n.z, D);
	}

	// Get the normal of this plane.
	paperback::Vector3f GetNormal() const
	{
		return paperback::Vector3f(mData.x, mData.y, mData.z);
	}
	// Get the 'd' value (the distance from the origin) which can be computed as Dot(origin - pointOnPlane, normal)
	float GetDistance() const
	{
		return mData.w;
	}

	// debugDraw>?

	paperback::Vector4f mData;


};

// ----- End of header guard
#endif