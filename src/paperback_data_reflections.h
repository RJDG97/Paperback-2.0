#pragma once
#include "glm/inc/glm.hpp"
#include "Math/Vector3f.h" 
#include "../Sandbox/Components/Deck.h"
#include "../Sandbox/Components/EntityScript.h"
#include "../Sandbox/Components/Cinematic.h"
#include "../Sandbox/Components/ParticleEmitter.h"

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

		rttr::registration::class_<button::ButtonTexture>("ButtonTextures")
			.constructor()(rttr::policy::ctor::as_object)
			.property("ID", &button::ButtonTexture::m_TextureName);

		rttr::registration::class_<cinematic::CinematicInfo>("CinematicInfo")
			.constructor()(rttr::policy::ctor::as_object)
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

		rttr::registration::class_< Generate_Lifetime >("Generate Lifetime")
			.constructor()(rttr::policy::ctor::as_object)
			.property("Min Lifetime", &Generate_Lifetime::m_Min)
			.property("Max Lifetime", &Generate_Lifetime::m_Max);
		
		rttr::registration::class_< Generate_Velocity >( "Generate Velocity" )
			.constructor()( rttr::policy::ctor::as_object )
			.property( "Min Velocity", &Generate_Velocity::m_Min )
			.property( "Max Velocity", &Generate_Velocity::m_Max );

		rttr::registration::class_< Generate_Rotation >( "Generate Rotation" )
			.constructor()( rttr::policy::ctor::as_object )
			.property( "Min Rotation", &Generate_Rotation::m_Min )
			.property( "Max Rotation", &Generate_Rotation::m_Max );

		rttr::registration::class_< Generate_Opacity >( "Generate Opacity" )
			.constructor()( rttr::policy::ctor::as_object )
			.property( "Min Opacity", &Generate_Opacity::m_Min )
			.property( "Max Opacity", &Generate_Opacity::m_Max );

		rttr::registration::class_< Update_Velocity >( "Update Velocity" )
			.constructor()( rttr::policy::ctor::as_object )
			.property( "Destination", &Update_Velocity::m_Destination )
			.property( "Min Velocity", &Update_Velocity::m_Min )
			.property( "Max Velocity", &Update_Velocity::m_Max );

		rttr::registration::class_< Generate_Scale >( "Generate Scale" )
			.constructor()( rttr::policy::ctor::as_object )
			.property( "Min Scale", &Generate_Scale::m_Min )
			.property( "Max Scale", &Generate_Scale::m_Max )
			.property( "Enable Uniform Scaling?", &Generate_Scale::m_bUniformScale );
	}
}