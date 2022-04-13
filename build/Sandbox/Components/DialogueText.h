/**********************************************************************************
*\file         DialogueText.h
*\brief        DialogueText.h
*
*\author       Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <string>

struct dialogue_text
{
	enum States
	{
		INACTIVE,
		OPENING,
		PLAYING,
		CLOSING
	};

	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "DialogueText"
	};

	float m_TextSpeed;
	float m_ElapsedTime;
	float m_HoldTime;
	bool m_OnHold;
	std::string m_DialogueName;
	paperback::Vector3f m_InitialScale;
	paperback::u32 m_CurrentIndex;
	int m_State;					//0 - inactivate, 1 - opening, 2 - playing, 3 - closing
};

namespace RR_DialogueText
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<dialogue_text>(dialogue_text::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
			.property("Text Speed", &dialogue_text::m_TextSpeed)(rttr::policy::prop::as_reference_wrapper)
			.property("Elapsed Time", &dialogue_text::m_ElapsedTime)
			.property("Hold Time", &dialogue_text::m_HoldTime)(rttr::policy::prop::as_reference_wrapper)
			.property("On Hold", &dialogue_text::m_OnHold)
			.property("Dialogue Name", &dialogue_text::m_DialogueName)(rttr::policy::prop::as_reference_wrapper)
			.property("Initial Scale", &dialogue_text::m_InitialScale)(rttr::policy::prop::as_reference_wrapper)
			.property("Current Index", &dialogue_text::m_CurrentIndex)
			.property("State", &dialogue_text::m_State)(rttr::policy::prop::as_reference_wrapper);
	}
}