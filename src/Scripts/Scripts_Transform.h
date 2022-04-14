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

	MONO_EXPORT paperback::Vector3f GetOffset(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<transform>(m_obj.m_PoolDetails)->m_Offset;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetPosition(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<transform>(m_obj.m_PoolDetails)->m_Position;

		return {};
	}

	MONO_EXPORT void SetOffset(uint32_t ID, float x, float y, float z)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<transform>(m_obj.m_PoolDetails)->m_Offset = {x, y, z};
	}

	MONO_EXPORT void SetPosition(uint32_t ID, float x, float y, float z)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<transform>(m_obj.m_PoolDetails)->m_Position = { x, y, z };
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Transform::getaddress(uint)", &MONO_TRANSFORM::GetAddress);
		mono_add_internal_call("CSScript.Transform::getoffset(uint)", &MONO_TRANSFORM::GetOffset);
		mono_add_internal_call("CSScript.Transform::setoffset(uint,single,single,single)", &MONO_TRANSFORM::SetOffset);
		mono_add_internal_call("CSScript.Transform::getposition(uint)", &MONO_TRANSFORM::GetPosition);
		mono_add_internal_call("CSScript.Transform::setposition(uint,single,single,single)", &MONO_TRANSFORM::SetPosition);
	}
}