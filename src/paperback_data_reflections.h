#pragma once
#include "glm/inc/glm.hpp"
#include "Math/Vector3f.h" 
#include "../Sandbox/Components/Deck.h"
#include "../Sandbox/Components/EntityScript.h"
#include "../Sandbox/Components/Cinematic.h"

namespace paperback
{
	// register the math libs here
	RTTR_REGISTRATION
	{
		rttr::registration::class_<CardDetails>("CardDetails")
			.constructor()(rttr::policy::ctor::as_object)
			.property("Card GID", &CardDetails::m_CardGID)
			.property("Count", &CardDetails::m_Count);

		rttr::registration::class_<entityscript::ScriptID>("ScriptID")
			.constructor()(rttr::policy::ctor::as_object)
			.property("ID", &entityscript::ScriptID::m_ID);

		rttr::registration::class_<cinematic::CinematicInfo>("CinematicInfo")
			.constructor()(rttr::policy::ctor::as_object)
			.property("Position", &cinematic::CinematicInfo::m_CamPosition)
			.property("Hold Time", &cinematic::CinematicInfo::m_HoldTime)
			.property("Move Speed", &cinematic::CinematicInfo::m_MoveSpeed);

		rttr::registration::class_<paperback::Vector3f>("Vector3f")
			.constructor()(rttr::policy::ctor::as_object)
			.property("x", &paperback::Vector3f::x)
			.property("y", &paperback::Vector3f::y)
			.property("z", &paperback::Vector3f::z);

		rttr::registration::class_<xcore::math::vector2>("Vector2")
			.constructor()(rttr::policy::ctor::as_object)
			.property("x", &xcore::math::vector2::m_X)
			.property("y", &xcore::math::vector2::m_Y);

		rttr::registration::class_<xcore::math::vector3d>("Vector3d")
			.constructor()(rttr::policy::ctor::as_object)
			.property("x", &xcore::math::vector3d::m_X)
			.property("y", &xcore::math::vector3d::m_Y)
			.property("z", &xcore::math::vector3d::m_Z);

		rttr::registration::class_<xcore::math::vector3>("Vector3")
			.constructor()(rttr::policy::ctor::as_object)
			.property("x", &xcore::math::vector3::m_X)
			.property("y", &xcore::math::vector3::m_Y)
			.property("z", &xcore::math::vector3::m_Z)
			.property("w", &xcore::math::vector3::m_W);

		//rttr::registration::class_<glm::vec3>("glm_Vector3")
		//	.constructor()(rttr::policy::ctor::as_object)
		//	.property("x", &glm::vec3::x)
		//	.property("y", &glm::vec3::y)
		//	.property("z", &glm::vec3::z);
	}
}