// ----- Start of header guard
#ifndef BOUNDIINGBOX_H
#define BOUNDIINGBOX_H

#include "Math/Vector3f.h"

struct BoundingBox
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "BoundingBox"
	};

public:													// ----- Private members
	paperback::Vector3f MinMax[2];
	bool collided;

	BoundingBox() : 
		collided{ false }
	{
		MinMax[0] = MinMax[1] = paperback::Vector3f{};
	}
	

};
// -- To do:
//		center of BoundingBox
//		this will mainly be used as a 1st/2nd step collision check
//			IN PRINCIPLE, ITS SIMILAR TO AABB, but it'll not be used as AABB - there's a specific struct for this

// ----- End of header guard
#endif