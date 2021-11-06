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

	std::string parent_socket;
	glm::mat4 bone_transform;


};

namespace RR_SOCKETED
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<socketed>(socketed::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Parent Socket", &socketed::parent_socket);
	}
}
#endif