// ----- Start of header guard
#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Math/Vector3f.h"

// -- Important Note: All line segments must follow and close the polygon shape in a 
struct LineSegment
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "LineSegment"
	};

private:													// ----- Private members
	paperback::Vector3f m_ptStart;										// ----- Start pt of segment
	paperback::Vector3f m_ptEnd;										// ----- End pt of segment
public:														// ----- Public members
	LineSegment(const paperback::Vector3f& _ptStart, const paperback::Vector3f& _ptEnd)			// ----- Class constructor
		: m_ptStart{ _ptStart }, m_ptEnd{ _ptEnd }
	{
	}
	inline void Set(paperback::Vector3f& _ptStart, paperback::Vector3f& _ptEnd)					// ----- Line segment set function
	{
		m_ptStart = _ptStart;
		m_ptEnd = _ptEnd;
	}
	inline paperback::Vector3f getStart() const { return m_ptStart; }	// ----- Returns the ptStart of the Line Segment
	inline paperback::Vector3f getEnd() const { return m_ptEnd; }	    // ----- Returns the ptEnd of the Line Segment
	void setStart(const paperback::Vector3f& lhs) { m_ptStart = lhs; }
	void setEnd(const paperback::Vector3f& lhs) { m_ptEnd = lhs; }
};

// for the normal, left hand, -> thumb x center finger = index finger
//   or               (negative)  -center finger x thumb = index finger

// ----- End of header guard
#endif