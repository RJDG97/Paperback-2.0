#pragma once
#ifndef SOCKETED_H
#define SOCKETED_H

#include "glm/inc/glm.hpp"
#include <string>

struct socketed
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Socketed"
	};

	std::string m_ParentSocket;
	glm::mat4 m_BoneTransform;
	paperback::Vector3f	 m_SocketPosOffset;
	paperback::Vector3f	 m_SocketRotOffset;
	paperback::Vector3f	 m_SocketScaleOffset;
	bool m_SyncAnimationWithParent;

};

namespace RR_SOCKETED
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<socketed>(socketed::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Parent Socket", &socketed::m_ParentSocket)(rttr::policy::prop::as_reference_wrapper)
		    .property("Socket Postion Offset", &socketed::m_SocketPosOffset)(rttr::policy::prop::as_reference_wrapper)
		    .property("Socket Rotation Offset", &socketed::m_SocketRotOffset)(rttr::policy::prop::as_reference_wrapper)
		    .property("Socket Scale Offset", &socketed::m_SocketScaleOffset)(rttr::policy::prop::as_reference_wrapper)
			.property("Sync Animation With Parent", &socketed::m_SyncAnimationWithParent)(rttr::policy::prop::as_reference_wrapper);
	}
}
#endif