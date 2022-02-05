#pragma once

#include "Mono.h"

namespace MONO_ELEVATOR
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_elevator = m_obj.m_pArchetype->FindComponent<elevator>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_elevator)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Elevator component." << std::endl;
		}
#endif

		return m_elevator;
	}

	MONO_EXPORT int32_t GetStartFrame(void* address)
	{
		if (address)
			return reinterpret_cast<elevator*>(address)->m_StartFrame;

		return {};
	}

	MONO_EXPORT int32_t GetStopFrame(void* address)
	{
		if (address)
			return reinterpret_cast<elevator*>(address)->m_StopFrame;

		return {};
	}

	MONO_EXPORT bool GetActivated(void* address)
	{
		if (address)
			return reinterpret_cast<elevator*>(address)->m_Activated;

		return {};
	}

	MONO_EXPORT void SetStartFrame(void* address, int32_t value)
	{
		if (address)
			reinterpret_cast<elevator*>(address)->m_StartFrame = value;
	}

	MONO_EXPORT void SetStopFrame(void* address, int32_t value)
	{
		if (address)
			reinterpret_cast<elevator*>(address)->m_StopFrame = value;
	}

	MONO_EXPORT void SetActivated(void* address, bool value)
	{
		if (address)
			reinterpret_cast<elevator*>(address)->m_Activated = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Elevator::getaddress(uint)", &MONO_ELEVATOR::GetAddress);
		mono_add_internal_call("CSScript.Elevator::getstartframe(void*)", &MONO_ELEVATOR::GetStartFrame);
		mono_add_internal_call("CSScript.Elevator::getstopframe(void*)", &MONO_ELEVATOR::GetStopFrame);
		mono_add_internal_call("CSScript.Elevator::getactivated(void*)", &MONO_ELEVATOR::GetActivated);
		mono_add_internal_call("CSScript.Elevator::setstartframe(void*,int)", &MONO_ELEVATOR::SetStartFrame);
		mono_add_internal_call("CSScript.Elevator::setstopframe(void*,int)", &MONO_ELEVATOR::SetStopFrame);
		mono_add_internal_call("CSScript.Elevator::setactivated(void*,bool)", &MONO_ELEVATOR::SetActivated);
	}
}