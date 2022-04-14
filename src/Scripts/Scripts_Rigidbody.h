/**********************************************************************************
*\file         Scripts_Rigidbody.h
*\brief        Scripts_Rigidbody.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_RIGIDBODY
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_rigidbody = m_obj.m_pArchetype->FindComponent<rigidbody>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_rigidbody)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Rigidbody component." << std::endl;
		}
#endif

		return m_rigidbody;
	}
	/*
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

	MONO_EXPORT float VelocityMagnitudeSquared(void* address)
	{

		if (address)
			return reinterpret_cast<rigidbody*>(address)->m_Velocity.x;//.MagnitudeSq();
		else
			return 0.0f;
	}*/


	MONO_EXPORT paperback::Vector3f GetAccel(uint32_t ID)
	{

		auto obj = PPB.GetEntityInfo(ID);

		auto rigid = obj.m_pArchetype->FindComponent<rigidbody>(obj.m_PoolDetails);

		if (rigid)
			return rigid->m_Accel;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetVelocity(uint32_t ID)
	{

		auto obj = PPB.GetEntityInfo(ID);

		auto rigid = obj.m_pArchetype->FindComponent<rigidbody>(obj.m_PoolDetails);

		if (rigid)
			return rigid->m_Velocity;

		return {};
	}

	MONO_EXPORT void SetAccel(uint32_t ID, float X, float Y, float Z)
	{

		auto obj = PPB.GetEntityInfo(ID);

		auto rigid = obj.m_pArchetype->FindComponent<rigidbody>(obj.m_PoolDetails);

		if (rigid)
			rigid->m_Accel = {X, Y, Z};
	}

	MONO_EXPORT void SetVelocity(uint32_t ID, float X, float Y, float Z)
	{

		auto obj = PPB.GetEntityInfo(ID);

		auto rigid = obj.m_pArchetype->FindComponent<rigidbody>(obj.m_PoolDetails);

		if (rigid)
			rigid->m_Velocity = { X, Y, Z };
	}

	MONO_EXPORT float VelocityMagnitudeSquared(uint32_t ID)
	{

		auto obj = PPB.GetEntityInfo(ID);

		auto rigid = obj.m_pArchetype->FindComponent<rigidbody>(obj.m_PoolDetails);

		if (rigid)
			return rigid->m_Velocity.x;//.MagnitudeSq();
		else
			return 0.0f;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Rigidbody::getaddress(uint)", &MONO_RIGIDBODY::GetAddress);
		mono_add_internal_call("CSScript.Rigidbody::getaccel(uint)", &MONO_RIGIDBODY::GetAccel);
		mono_add_internal_call("CSScript.Rigidbody::getvelocity(uint)", &MONO_RIGIDBODY::GetVelocity);
		mono_add_internal_call("CSScript.Rigidbody::setaccel(uint,single,single,single)", &MONO_RIGIDBODY::SetAccel);
		mono_add_internal_call("CSScript.Rigidbody::setvelocity(uint,single,single,single)", &MONO_RIGIDBODY::SetVelocity);
		mono_add_internal_call("CSScript.Rigidbody::VelocityMagnitudeSquared(uint)", &MONO_RIGIDBODY::VelocityMagnitudeSquared);
	}
}