#pragma once

#include "Mono.h"

namespace MONO_RIGIDBODY
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_rigidbody = m_obj.m_pArchetype->GetComponent<rigidbody>(m_obj.m_PoolDetails);
		return &m_rigidbody;
	}

	MONO_EXPORT paperback::Vector3f GetAccel(void* address)
	{
		if (address)
			return reinterpret_cast<rigidbody*>(address)->m_Accel;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetVelocity(void* address)
	{
		if (address)
			return reinterpret_cast<rigidbody*>(address)->m_Velocity;

		return {};
	}

	MONO_EXPORT void SetAccel(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<rigidbody*>(address)->m_Accel = {x, y, z};
	}

	MONO_EXPORT void SetVelocity(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<rigidbody*>(address)->m_Velocity = { x, y, z };
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Rigidbody::getaddress(uint)", &MONO_RIGIDBODY::GetAddress);
		mono_add_internal_call("CSScript.Rigidbody::getaccel(void*)", &MONO_RIGIDBODY::GetAccel);
		mono_add_internal_call("CSScript.Rigidbody::getvelocity(void*)", &MONO_RIGIDBODY::GetVelocity);
		mono_add_internal_call("CSScript.Rigidbody::setaccel(void*,single,single,single)", &MONO_RIGIDBODY::SetAccel);
		mono_add_internal_call("CSScript.Rigidbody::setvelocity(void*,single,single,single)", &MONO_RIGIDBODY::SetVelocity);
	}
}