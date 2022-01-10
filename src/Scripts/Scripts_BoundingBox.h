#pragma once

#include "Mono.h"

namespace MONO_BOUNDINGBOX
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_boundingbox = m_obj.m_pArchetype->GetComponent<boundingbox>(m_obj.m_PoolDetails);
		return &m_boundingbox;
	}

	MONO_EXPORT paperback::Vector3f GetMin(void* address)
	{
		return reinterpret_cast<boundingbox*>(address)->Min;
	}

	MONO_EXPORT void SetMin(void* address, float x, float y, float z)
	{
		reinterpret_cast<boundingbox*>(address)->Min = {x, y, z};
	}

	MONO_EXPORT paperback::Vector3f GetMax(void* address)
	{
		return reinterpret_cast<boundingbox*>(address)->Max;
	}

	MONO_EXPORT void SetMax(void* address, float x, float y, float z)
	{
		reinterpret_cast<boundingbox*>(address)->Max = { x, y, z };
	}

	MONO_EXPORT bool GetCollided(void* address)
	{
		return reinterpret_cast<boundingbox*>(address)->m_Collided;
	}

	MONO_EXPORT std::vector<bool> GetCollidedState(void* address)
	{
		return reinterpret_cast<boundingbox*>(address)->m_CollisionState;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.BOUNDINGBOX::getaddress(uint)", &MONO_ANIMATOR::GetAddress);
		mono_add_internal_call("CSScript.BOUNDINGBOX::getmin(void*)", &MONO_ANIMATOR::GetMin);
		mono_add_internal_call("CSScript.BOUNDINGBOX::setmin(void*,single,single,single)", &MONO_ANIMATOR::SetMin);
		mono_add_internal_call("CSScript.BOUNDINGBOX::getmax(void*)", &MONO_ANIMATOR::GetMax);
		mono_add_internal_call("CSScript.BOUNDINGBOX::setmax(void*,single,single,single)", &MONO_ANIMATOR::SetMax);
		mono_add_internal_call("CSScript.BOUNDINGBOX::getcollided(void*)", &MONO_ANIMATOR::GetCollided);
		mono_add_internal_call("CSScript.BOUNDINGBOX::getcollisionstate(void*)", &MONO_ANIMATOR::GetCollidedState);
	}
}