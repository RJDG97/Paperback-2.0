/**********************************************************************************
*\file         Scripts_Transform.h
*\brief        Scripts_Transform.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_TRANSFORM
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_transform = m_obj.m_pArchetype->FindComponent<transform>(m_obj.m_PoolDetails);

		return m_transform;
	}

	MONO_EXPORT paperback::Vector3f GetOffset(void* address)
	{
		if (address)
			return reinterpret_cast<transform*>(address)->m_Offset;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetPosition(void* address)
	{
		if (address)
			return reinterpret_cast<transform*>(address)->m_Position;

		return {};
	}

	MONO_EXPORT void SetOffset(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<transform*>(address)->m_Offset = {x, y, z};
	}

	MONO_EXPORT void SetPosition(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<transform*>(address)->m_Position = { x, y, z };
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Transform::getaddress(uint)", &MONO_TRANSFORM::GetAddress);
		mono_add_internal_call("CSScript.Transform::getoffset(void*)", &MONO_TRANSFORM::GetOffset);
		mono_add_internal_call("CSScript.Transform::setoffset(void*,single,single,single)", &MONO_TRANSFORM::SetOffset);
		mono_add_internal_call("CSScript.Transform::getposition(void*)", &MONO_TRANSFORM::GetPosition);
		mono_add_internal_call("CSScript.Transform::setposition(void*,single,single,single)", &MONO_TRANSFORM::SetPosition);
	}
}