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

	MONO_EXPORT std::string GetCurrentAnimationName(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_CurrentAnimationName;

		return {};
	}

	MONO_EXPORT void SetCurrentAnimationName(uint32_t ID, MonoString* new_name)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_CurrentAnimationName = mono_string_to_utf8(new_name);
	}

	MONO_EXPORT float GetCurrentAnimationTime(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_CurrentTime;

		return {};
	}

	MONO_EXPORT void SetCurrentAnimationTime(uint32_t ID, float time)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_CurrentTime = time;
	}

	MONO_EXPORT bool GetPlayOnce(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_PlayOnce;

		return {};
	}

	MONO_EXPORT void SetPlayOnce(uint32_t ID, bool play_once)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_PlayOnce = play_once;
	}

	MONO_EXPORT bool GetFinishedAnimating(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_FinishedAnimating;

		return {};
	}

	MONO_EXPORT bool GetPauseAnimation(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_PauseAnimation;

		return {};
	}

	MONO_EXPORT void SetPauseAnimation(uint32_t ID, bool play_once)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_PauseAnimation = play_once;
	}

	MONO_EXPORT float GetPauseAtTime(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_PauseAtTime;

		return {};
	}

	MONO_EXPORT void SetPauseAtTime(uint32_t ID, float time)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_PauseAtTime = time;
	}

	MONO_EXPORT bool GetReversed(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_Reversed;

		return {};
	}

	MONO_EXPORT void SetReversed(uint32_t ID, bool reversed)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails)->m_Reversed = reversed;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Animator::getaddress(uint)", &MONO_ANIMATOR::GetAddress);
		mono_add_internal_call("CSScript.Animator::getcurrentanimationname(uint)", &MONO_ANIMATOR::GetCurrentAnimationName);
		mono_add_internal_call("CSScript.Animator::setcurrentanimationname(uint,string)", &MONO_ANIMATOR::SetCurrentAnimationName);
		mono_add_internal_call("CSScript.Animator::getcurrentanimationtime(uint)", &MONO_ANIMATOR::GetCurrentAnimationTime);
		mono_add_internal_call("CSScript.Animator::setcurrentanimationtime(uint,single)", &MONO_ANIMATOR::SetCurrentAnimationTime);
		mono_add_internal_call("CSScript.Animator::getplayonce(uint)", &MONO_ANIMATOR::GetPlayOnce);
		mono_add_internal_call("CSScript.Animator::setplayonce(uint,bool)", &MONO_ANIMATOR::SetPlayOnce);
		mono_add_internal_call("CSScript.Animator::getfinishedanimating(uint)", &MONO_ANIMATOR::GetFinishedAnimating);
		mono_add_internal_call("CSScript.Animator::getpauseanimation(uint)", &MONO_ANIMATOR::GetPauseAnimation);
		mono_add_internal_call("CSScript.Animator::setpauseanimation(uint,bool)", &MONO_ANIMATOR::SetPauseAnimation);
		mono_add_internal_call("CSScript.Animator::getpauseattime(uint)", &MONO_ANIMATOR::GetPauseAtTime);
		mono_add_internal_call("CSScript.Animator::setpauseattime(uint,single)", &MONO_ANIMATOR::SetPauseAtTime);
		mono_add_internal_call("CSScript.Animator::getreversed(uint)", &MONO_ANIMATOR::GetReversed);
		mono_add_internal_call("CSScript.Animator::setreversed(uint,bool)", &MONO_ANIMATOR::SetReversed);
	}
}