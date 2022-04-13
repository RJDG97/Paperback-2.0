/**********************************************************************************
*\file         Scripts_Scale.h
*\brief        Scripts_Scale.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_SCALE
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_scale = m_obj.m_pArchetype->FindComponent<scale>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_scale)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Scale component." << std::endl;
		}
#endif

		return m_scale;
	}

	MONO_EXPORT paperback::Vector3f GetValue(void* address)
	{
		if (address)
			return reinterpret_cast<scale*>(address)->m_Value;

		return {};
	}

	MONO_EXPORT void SetValue(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<scale*>(address)->m_Value = {x, y, z};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Scale::getaddress(uint)", &MONO_SCALE::GetAddress);
		mono_add_internal_call("CSScript.Scale::getvalue(void*)", &MONO_SCALE::GetValue);
		mono_add_internal_call("CSScript.Scale::setvalue(void*,single,single,single)", &MONO_SCALE::SetValue);
	}
}