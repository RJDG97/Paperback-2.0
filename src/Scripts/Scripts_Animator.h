/**********************************************************************************
*\file         Scripts_Animator.h
*\brief        Scripts_Animator.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_ANIMATOR
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_animator = m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_animator)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Animator component." << std::endl;
		}
#endif

		return m_animator;
	}

	MONO_EXPORT std::string GetCurrentAnimationName(void* address)
	{
		if (address)
			return reinterpret_cast<animator*>(address)->m_CurrentAnimationName;

		return {};
	}

	MONO_EXPORT void SetCurrentAnimationName(void* address, MonoString* new_name)
	{
		if (address)
			reinterpret_cast<animator*>(address)->m_CurrentAnimationName = mono_string_to_utf8(new_name);
	}

	MONO_EXPORT float GetCurrentAnimationTime(void* address)
	{
		if (address)
			return reinterpret_cast<animator*>(address)->m_CurrentTime;

		return {};
	}

	MONO_EXPORT void SetCurrentAnimationTime(void* address, float time)
	{
		if (address)
			reinterpret_cast<animator*>(address)->m_CurrentTime = time;
	}

	MONO_EXPORT bool GetPlayOnce(void* address)
	{
		if (address)
			return reinterpret_cast<animator*>(address)->m_PlayOnce;

		return {};
	}

	MONO_EXPORT void SetPlayOnce(void* address, bool play_once)
	{
		if (address)
			reinterpret_cast<animator*>(address)->m_PlayOnce = play_once;
	}

	MONO_EXPORT bool GetFinishedAnimating(void* address)
	{
		if (address)
			return reinterpret_cast<animator*>(address)->m_FinishedAnimating;

		return {};
	}

	MONO_EXPORT bool GetPauseAnimation(void* address)
	{
		if (address)
			return reinterpret_cast<animator*>(address)->m_PauseAnimation;

		return {};
	}

	MONO_EXPORT void SetPauseAnimation(void* address, bool play_once)
	{
		if (address)
			reinterpret_cast<animator*>(address)->m_PauseAnimation = play_once;
	}

	MONO_EXPORT float GetPauseAtTime(void* address)
	{
		if (address)
			return reinterpret_cast<animator*>(address)->m_PauseAtTime;

		return {};
	}

	MONO_EXPORT void SetPauseAtTime(void* address, float time)
	{
		if (address)
			reinterpret_cast<animator*>(address)->m_PauseAtTime = time;
	}

	MONO_EXPORT bool GetReversed(void* address)
	{
		if (address)
			return reinterpret_cast<animator*>(address)->m_Reversed;

		return {};
	}

	MONO_EXPORT void SetReversed(void* address, bool reversed)
	{
		if (address)
			reinterpret_cast<animator*>(address)->m_Reversed = reversed;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Animator::getaddress(uint)", &MONO_ANIMATOR::GetAddress);
		mono_add_internal_call("CSScript.Animator::getcurrentanimationname(void*)", &MONO_ANIMATOR::GetCurrentAnimationName);
		mono_add_internal_call("CSScript.Animator::setcurrentanimationname(void*,string)", &MONO_ANIMATOR::SetCurrentAnimationName);
		mono_add_internal_call("CSScript.Animator::getcurrentanimationtime(void*)", &MONO_ANIMATOR::GetCurrentAnimationTime);
		mono_add_internal_call("CSScript.Animator::setcurrentanimationtime(void*,single)", &MONO_ANIMATOR::SetCurrentAnimationTime);
		mono_add_internal_call("CSScript.Animator::getplayonce(void*)", &MONO_ANIMATOR::GetPlayOnce);
		mono_add_internal_call("CSScript.Animator::setplayonce(void*,bool)", &MONO_ANIMATOR::SetPlayOnce);
		mono_add_internal_call("CSScript.Animator::getfinishedanimating(void*)", &MONO_ANIMATOR::GetFinishedAnimating);
		mono_add_internal_call("CSScript.Animator::getpauseanimation(void*)", &MONO_ANIMATOR::GetPauseAnimation);
		mono_add_internal_call("CSScript.Animator::setpauseanimation(void*,bool)", &MONO_ANIMATOR::SetPauseAnimation);
		mono_add_internal_call("CSScript.Animator::getpauseattime(void*)", &MONO_ANIMATOR::GetPauseAtTime);
		mono_add_internal_call("CSScript.Animator::setpauseattime(void*,single)", &MONO_ANIMATOR::SetPauseAtTime);
		mono_add_internal_call("CSScript.Animator::getreversed(void*)", &MONO_ANIMATOR::GetReversed);
		mono_add_internal_call("CSScript.Animator::setreversed(void*,bool)", &MONO_ANIMATOR::SetReversed);
	}
}