#pragma once

#include "Mono.h"

namespace MONO_SOCKETED
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_socketed = m_obj.m_pArchetype->GetComponent<socketed>(m_obj.m_PoolDetails);
		return &m_socketed;
	}

	MONO_EXPORT std::string GetParentSocket(void* address)
	{
		if (address)
			return reinterpret_cast<socketed*>(address)->m_ParentSocket;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetSocketPosOffset(void* address)
	{
		if (address)
			return reinterpret_cast<socketed*>(address)->m_SocketPosOffset;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetSocketRotOffset(void* address)
	{
		if (address)
			return reinterpret_cast<socketed*>(address)->m_SocketRotOffset;

		return {};
	}

	MONO_EXPORT paperback::Vector3f GetSocketScaleOffset(void* address)
	{
		if (address)
			return reinterpret_cast<socketed*>(address)->m_SocketScaleOffset;

		return {};
	}

	MONO_EXPORT void SetParentSocket(void* address, std::string value)
	{
		if (address)
			reinterpret_cast<socketed*>(address)->m_ParentSocket = value;
	}

	MONO_EXPORT void SetSocketPosOffset(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<socketed*>(address)->m_SocketPosOffset = { x, y, z };
	}

	MONO_EXPORT void SetSocketRotOffset(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<socketed*>(address)->m_SocketRotOffset = { x, y, z };
	}

	MONO_EXPORT void SetSocketScaleOffset(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<socketed*>(address)->m_SocketScaleOffset = { x, y, z };
	}


	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Socketed::getaddress(uint)", &MONO_SOCKETED::GetAddress);
		mono_add_internal_call("CSScript.Socketed::getparentsocket(void*)", &MONO_SOCKETED::GetSocketPosOffset);
		mono_add_internal_call("CSScript.Socketed::getsocketposoffset(void*)", &MONO_SOCKETED::GetSocketRotOffset);
		mono_add_internal_call("CSScript.Socketed::getsocketrotoffset(void*)", &MONO_SOCKETED::GetOffset);
		mono_add_internal_call("CSScript.Socketed::getsocketscaleoffset(void*)", &MONO_SOCKETED::GetOffset);
		mono_add_internal_call("CSScript.Socketed::setparentsocket(void*,string)", &MONO_SOCKETED::SetParentSocket);
		mono_add_internal_call("CSScript.Socketed::setsocketposoffset(void*,single,single,single)", &MONO_SOCKETED::SetSocketPosOffset);
		mono_add_internal_call("CSScript.Socketed::setsocketrotoffset(void*,single,single,single)", &MONO_SOCKETED::SetSocketRotOffset);
		mono_add_internal_call("CSScript.Socketed::setsocketscaleoffset(void*,single,single,single)", &MONO_SOCKETED::SetSocketScaleOffset);
	}
}