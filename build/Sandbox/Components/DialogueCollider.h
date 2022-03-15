#pragma once

#include <string>

struct dialogue_collider
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "DialogueCollider"
	};

	std::string m_DialogueName;
};

namespace RR_DialogueCollider
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<dialogue_collider>(dialogue_collider::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
			.property("Dialogue Name", &dialogue_collider::m_DialogueName)(rttr::policy::prop::as_reference_wrapper);
	}
}