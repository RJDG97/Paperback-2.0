// ----- Start of header guard
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Math/Vector3f.h"

struct Circle
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Circle"
	};

private:													// ----- Private members
	paperback::Vector3f		m_ptCenter;							// ----- Center point
	float				m_fRadius;							// ----- Radius of circle
public:														// ----- Public members
	Circle(const paperback::Vector3f& center, float radius)			// ----- Circle constructor
		: m_ptCenter(center)
		, m_fRadius(radius)
	{}
	inline void Set(paperback::Vector3f center, float radius)			// ----- Circle set function
	{
		m_ptCenter = center;
		m_fRadius = radius;
	}
	inline paperback::Vector3f gettCenter() const { return m_ptCenter; }	// ----- Returns the point center of the Circle
	inline float getRadius() const { return m_fRadius; }		// ----- Returns the point center of the Circle
	void setCenter(const paperback::Vector3f& lhs) { m_ptCenter = lhs; }// ----- Returns the point center of the Circle
	void setRadius(float raii) { m_fRadius = raii; }			// ----- Returns the point center of the Circle
};
// ----- End of header guard
#endif
