#pragma once
#ifndef BOUNDIINGBOX_H
#define BOUNDIINGBOX_H

#include "Math/Vector3f.h"

struct boundingbox
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "BoundingBox"
	};

	paperback::Vector3f Min, Max;
	bool m_Collided;
	std::unordered_map<uint32_t, bool> m_CollisionState;

	// used in debugDraw
	boundingbox(
		paperback::Vector3f minimum = paperback::Vector3f{},
		paperback::Vector3f maximum = paperback::Vector3f{},
		bool collide = false) :
		Min{ minimum },
		Max{ maximum },
		m_Collided{ collide }
	{}
};
// -- To do:
//		center of BoundingBox
//		this will mainly be used as a 1st/2nd step collision check
//			IN PRINCIPLE, ITS SIMILAR TO AABB, but it'll not be used as AABB - there's a specific struct for this

namespace RR_BOUNDINGBOX
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<boundingbox>(boundingbox::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Min Point", &boundingbox::Min)(rttr::policy::prop::as_reference_wrapper)
		   .property("Max Point", &boundingbox::Max)(rttr::policy::prop::as_reference_wrapper)
		   .property("Is Collide", &boundingbox::m_Collided);
	}
}
// ----- End of header guard
#endif