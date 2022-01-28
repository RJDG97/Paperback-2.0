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
		if (address)
			return reinterpret_cast<animator*>(address)->m_CurrentAnimationName;

		return {};
	}

	MONO_EXPORT void SetCurrentAnimationName(void* address, std::string new_name)
	{
		if (address)
			reinterpret_cast<animator*>(address)->m_CurrentAnimationName = new_name;
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

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.ANIMATOR::getaddress(uint)", &MONO_ANIMATOR::GetAddress);
		mono_add_internal_call("CSScript.ANIMATOR::getcurrentanimationname(void*)", &MONO_ANIMATOR::GetCurrentAnimationName);
		mono_add_internal_call("CSScript.ANIMATOR::setcurrentanimationname(void*,string)", &MONO_ANIMATOR::SetCurrentAnimationName);
		mono_add_internal_call("CSScript.ANIMATOR::getcurrentanimationtime(void*)", &MONO_ANIMATOR::GetCurrentAnimationTime);
		mono_add_internal_call("CSScript.ANIMATOR::setcurrentanimationtime(void*,single)", &MONO_ANIMATOR::SetCurrentAnimationTime);
		mono_add_internal_call("CSScript.ANIMATOR::getplayonce(void*)", &MONO_ANIMATOR::GetPlayOnce);
		mono_add_internal_call("CSScript.ANIMATOR::setplayonce(void*,bool)", &MONO_ANIMATOR::SetPlayOnce);
		mono_add_internal_call("CSScript.ANIMATOR::getfinishedanimating(void*)", &MONO_ANIMATOR::GetFinishedAnimating);
		mono_add_internal_call("CSScript.ANIMATOR::getpauseanimation(void*)", &MONO_ANIMATOR::GetPauseAnimation);
		mono_add_internal_call("CSScript.ANIMATOR::setpauseanimation(void*,bool)", &MONO_ANIMATOR::SetPauseAnimation);
	}
}