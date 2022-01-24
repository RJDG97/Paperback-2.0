#pragma once

#include "Mono.h"

namespace MONO_SOUND
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_sound = m_obj.m_pArchetype->GetComponent<sound>(m_obj.m_PoolDetails);
		return &m_sound;
	}

	MONO_EXPORT std::string GetSoundID(void* address)
	{
		if (address)
			return reinterpret_cast<sound*>(address)->m_SoundID;

		return {};
	}

	MONO_EXPORT void SetSoundID(void* address, std::string value)
	{
		if (address)
			reinterpret_cast<sound*>(address)->m_SoundID = value;
	}

	MONO_EXPORT uint32_t GetSoundPlayTag(void* address)
	{
		if (address)
			return static_cast<uint32_t>(reinterpret_cast<sound*>(address)->m_SoundPlayTag);

		return {};
	}

	MONO_EXPORT void SetSoundPlayTag(void* address, uint32_t value)
	{
		if (address)
			reinterpret_cast<sound*>(address)->m_SoundPlayTag = value;
	}

	MONO_EXPORT bool GetIs3DSound(void* address)
	{
		if (address)
			return reinterpret_cast<sound*>(address)->m_Is3DSound;

		return {};
	}

	MONO_EXPORT void SetIs3DSound(void* address, bool value)
	{
		if (address)
			reinterpret_cast<sound*>(address)->m_Is3DSound = value;
	}

	MONO_EXPORT bool GetIsTriggerable(void* address)
	{
		if (address)
			return reinterpret_cast<sound*>(address)->m_IsTriggerable;

		return {};
	}

	MONO_EXPORT void SetIsTriggerable(void* address, bool value)
	{
		if (address)
			reinterpret_cast<sound*>(address)->m_IsTriggerable = value;
	}

	MONO_EXPORT bool GetTrigger(void* address)
	{
		if (address)
			return reinterpret_cast<sound*>(address)->m_Trigger;

		return {};
	}

	MONO_EXPORT void SetTrigger(void* address, bool value)
	{
		if (address)
			reinterpret_cast<sound*>(address)->m_Trigger = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Sound::getaddress(uint)", &MONO_SOUND::GetAddress);
		mono_add_internal_call("CSScript.Sound::getsoundid(void*)", &MONO_SOUND::GetSoundID);
		mono_add_internal_call("CSScript.Sound::setsoundid(void*,string)", &MONO_SOUND::SetSoundID);
		mono_add_internal_call("CSScript.Sound::getsoundplaytag(void*)", &MONO_SOUND::GetSoundPlayTag);
		mono_add_internal_call("CSScript.Sound::setsoundplaytag(void*,uint)", &MONO_SOUND::SetSoundPlayTag);
		mono_add_internal_call("CSScript.Sound::getis3dsound(void*)", &MONO_SOUND::GetIs3DSound);
		mono_add_internal_call("CSScript.Sound::setis3dsound(void*,bool)", &MONO_SOUND::SetIs3DSound);
		mono_add_internal_call("CSScript.Sound::getistriggerable(void*)", &MONO_SOUND::GetIsTriggerable);
		mono_add_internal_call("CSScript.Sound::setistriggerable(void*,bool)", &MONO_SOUND::SetIsTriggerable);
		mono_add_internal_call("CSScript.Sound::gettrigger(void*)", &MONO_SOUND::GetTrigger);
		mono_add_internal_call("CSScript.Sound::settrigger(void*,bool)", &MONO_SOUND::SetTrigger);
	}
}