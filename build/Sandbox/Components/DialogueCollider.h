/**********************************************************************************
*\file         DialogueCollider.h
*\brief        DialogueCollider.h
*
*\author       Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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