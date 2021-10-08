// ----- Start of header guard
#ifndef SPHERE_H
#define SPHERE_H

#include "Math/Vector3f.h"
#include "Math/MathUtils.h"

struct Sphere
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Sphere"
	};

	paperback::Vector3f		m_ptCenter;							// ----- Center point
	float				m_fRadius;							// ----- Radius of circle
	bool				m_Collided;							// ----- collided

	Sphere()												// ----- default constructor
		: m_ptCenter(paperback::Vector3f{})
		, m_fRadius(0.f),
		m_Collided{ false }
	{

	}
	Sphere(const paperback::Vector3f& center, float radius)			// ----- Sphere constructor
		: m_ptCenter(center)
		, m_fRadius(radius)
	{}
	inline void Set(paperback::Vector3f center, float radius)			// ----- Sphere set function
	{
		m_ptCenter = center;
		m_fRadius = radius;
	}
	inline paperback::Vector3f getCenter() const { return m_ptCenter; }	// ----- Returns the point center of the Sphere
	inline float getRadius() const { return m_fRadius; }		// ----- Returns the radius of the Sphere
	void setCenter(const paperback::Vector3f& lhs) { m_ptCenter = lhs; }// ----- set the point center of the Sphere
	void setRadius(float raii) { m_fRadius = raii; }			// ----- set the radius of the Sphere
};

//		this will mainly be used as a 1st/2nd step collision check

// ----- End of header guard
#endif
