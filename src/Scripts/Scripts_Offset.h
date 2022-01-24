#pragma once

#include "Mono.h"

namespace MONO_OFFSET
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_offset = m_obj.m_pArchetype->GetComponent<offset>(m_obj.m_PoolDetails);
		return &m_offset;
	}

	MONO_EXPORT paperback::Vector3f GetPosOffset(void* address)
	{
		if (address)
			return reinterpret_cast<offset*>(address)->m_PosOffset;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetRotOffset(void* address)
	{
		if (address)
			return reinterpret_cast<offset*>(address)->m_RotOffset;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetScaleOffset(void* address)
	{
		if (address)
			return reinterpret_cast<offset*>(address)->m_ScaleOffset;

		return {};
	}

	MONO_EXPORT void SetPosOffset(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<offset*>(address)->m_PosOffset = {x, y, z};
	}

	MONO_EXPORT void SetRotOffset(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<offset*>(address)->m_RotOffset = { x, y, z };
	}

	MONO_EXPORT void SetScaleOffset(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<offset*>(address)->m_ScaleOffset = { x, y, z };
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Offset::getaddress(uint)", &MONO_OFFSET::GetAddress);
		mono_add_internal_call("CSScript.Offset::getposoffset(void*)", &MONO_OFFSET::GetPosOffset);
		mono_add_internal_call("CSScript.Offset::setposoffset(void*,single,single,single)", &MONO_OFFSET::SetPosOffset);
		mono_add_internal_call("CSScript.Offset::getrotoffset(void*)", &MONO_OFFSET::GetRotOffset);
		mono_add_internal_call("CSScript.Offset::setrotoffset(void*,single,single,single)", &MONO_OFFSET::SetRotOffset);
		mono_add_internal_call("CSScript.Offset::getscaleoffset(void*)", &MONO_OFFSET::GetScaleOffset);
		mono_add_internal_call("CSScript.Offset::setscaleoffset(void*,single,single,single)", &MONO_OFFSET::SetScaleOffset);
	}
}