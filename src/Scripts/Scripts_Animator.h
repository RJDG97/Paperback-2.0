#pragma once

#include "Mono.h"

namespace MONO_ANIMATOR
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_animator = m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
		return &m_animator;
	}

	MONO_EXPORT std::string GetCurrentAnimationName(void* address)
	{
		return reinterpret_cast<animator*>(address)->m_CurrentAnimationName;
	}

	MONO_EXPORT void SetCurrentAnimationName(void* address, std::string new_name)
	{
		reinterpret_cast<animator*>(address)->m_CurrentAnimationName = new_name;
	}

	MONO_EXPORT std::string GetCurrentTime(void* address)
	{
		return reinterpret_cast<animator*>(address)->m_CurrentTime;
	}

	MONO_EXPORT void SetCurrentTime(void* address, float time)
	{
		reinterpret_cast<animator*>(address)->m_CurrentTime = time;
	}

	MONO_EXPORT std::string GetPlayOnce(void* address)
	{
		return reinterpret_cast<animator*>(address)->m_PlayOnce;
	}

	MONO_EXPORT void SetPlayOnce(void* address, bool play_once)
	{
		reinterpret_cast<animator*>(address)->m_PlayOnce = play_once;
	}

	MONO_EXPORT std::string GetFinishedAnimating(void* address)
	{
		return reinterpret_cast<animator*>(address)->m_FinishedAnimating;
	}

	MONO_EXPORT std::string GetPauseAnimation(void* address)
	{
		return reinterpret_cast<animator*>(address)->m_PauseAnimation;
	}

	MONO_EXPORT void SetPauseAnimation(void* address, bool play_once)
	{
		reinterpret_cast<animator*>(address)->m_PauseAnimation = play_once;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.ANIMATOR::getaddress(uint)", &MONO_ANIMATOR::GetAddress);
		mono_add_internal_call("CSScript.ANIMATOR::getcurrentanimationname(void*)", &MONO_ANIMATOR::GetCurrentAnimationName);
		mono_add_internal_call("CSScript.ANIMATOR::setcurrentanimationname(void*,string)", &MONO_ANIMATOR::SetCurrentAnimationName);
		mono_add_internal_call("CSScript.ANIMATOR::getcurrenttime(void*)", &MONO_ANIMATOR::GetCurrentTime);
		mono_add_internal_call("CSScript.ANIMATOR::setcurrenttime(void*,single)", &MONO_ANIMATOR::SetCurrentTime);
		mono_add_internal_call("CSScript.ANIMATOR::getplayonce(void*)", &MONO_ANIMATOR::GetPlayOnce);
		mono_add_internal_call("CSScript.ANIMATOR::setplayonce(void*,bool)", &MONO_ANIMATOR::SetPlayOnce);
		mono_add_internal_call("CSScript.ANIMATOR::getfinishedanimating(void*)", &MONO_ANIMATOR::GetFinishedAnimating);
		mono_add_internal_call("CSScript.ANIMATOR::getpauseanimation(void*)", &MONO_ANIMATOR::GetPauseAnimation);
		mono_add_internal_call("CSScript.ANIMATOR::setpauseanimation(void*,bool)", &MONO_ANIMATOR::SetPauseAnimation);
	}
}