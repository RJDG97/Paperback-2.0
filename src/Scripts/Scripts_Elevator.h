/**********************************************************************************
*\file         Scripts_Elevator.h
*\brief        Scripts_Elevator.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

	MONO_EXPORT float GetStartTime(void* address)
	{
		if (address)
			return reinterpret_cast<elevator*>(address)->m_StartTime;

		return {};
	}

	MONO_EXPORT float GetStopTime(void* address)
	{
		if (address)
			return reinterpret_cast<elevator*>(address)->m_StopTime;

		return {};
	}

	MONO_EXPORT bool GetUnitUnder(void* address)
	{
		if (address)
			return reinterpret_cast<elevator*>(address)->m_UnitUnder;

		return {};
	}

	MONO_EXPORT void SetStartTime(void* address, float value)
	{
		if (address)
			reinterpret_cast<elevator*>(address)->m_StartTime = value;
	}

	MONO_EXPORT void SetStopTime(void* address, float value)
	{
		if (address)
			reinterpret_cast<elevator*>(address)->m_StopTime = value;
	}

	MONO_EXPORT void SetUnitUnder(void* address, bool value)
	{
		if (address)
			reinterpret_cast<elevator*>(address)->m_UnitUnder = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Elevator::getaddress(uint)", &MONO_ELEVATOR::GetAddress);
		mono_add_internal_call("CSScript.Elevator::getstarttime(void*)", &MONO_ELEVATOR::GetStartTime);
		mono_add_internal_call("CSScript.Elevator::getstoptime(void*)", &MONO_ELEVATOR::GetStopTime);
		mono_add_internal_call("CSScript.Elevator::getunitunder(void*)", &MONO_ELEVATOR::GetUnitUnder);
		mono_add_internal_call("CSScript.Elevator::setstarttime(void*,single)", &MONO_ELEVATOR::SetStartTime);
		mono_add_internal_call("CSScript.Elevator::setstoptime(void*,single)", &MONO_ELEVATOR::SetStopTime);
		mono_add_internal_call("CSScript.Elevator::setunitunder(void*,bool)", &MONO_ELEVATOR::SetUnitUnder);
	}
}