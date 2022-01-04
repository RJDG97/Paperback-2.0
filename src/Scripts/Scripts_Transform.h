#pragma once

#include "Mono.h"

namespace MONO_TRANSFORM
{

	MONO_EXPORT paperback::Vector3f GetOffset(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto m_transform = m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
		return m_transform.m_Offset;
	}

	MONO_EXPORT paperback::Vector3f GetPosition(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto m_transform = m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
		return m_transform.m_Position;
	}

	MONO_EXPORT void SetOffset(uint32_t ID, float x, float y, float z)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto m_transform = m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
		m_transform.m_Offset = {x, y, z};
	}

	MONO_EXPORT void SetPosition(uint32_t ID, float x, float y, float z)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto m_transform = m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
		m_transform.m_Position = { x, y, z };
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Transform::getoffset(uint)", &MONO_TRANSFORM::GetOffset);
		mono_add_internal_call("CSScript.Transform::setoffset(uint,single,single,single)", &MONO_TRANSFORM::SetOffset);
		mono_add_internal_call("CSScript.Transform::getposition(uint)", &MONO_TRANSFORM::GetPosition);
		mono_add_internal_call("CSScript.Transform::setposition(uint,single,single,single)", &MONO_TRANSFORM::SetPosition);
	}
}