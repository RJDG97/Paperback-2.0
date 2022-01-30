#pragma once

#include "Mono.h"

namespace MONO_CAMERA
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_camera = m_obj.m_pArchetype->FindComponent<camera>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_camera)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Camera component." << std::endl;
		}
#endif

		return m_camera;
	}

	MONO_EXPORT bool GetActive(void* address)
	{
		if (address)
			return reinterpret_cast<camera*>(address)->m_Active;

		return {};
	}

	MONO_EXPORT void SetActive(void* address, bool value)
	{
		if (address)
			reinterpret_cast<camera*>(address)->m_Active = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Camera::getaddress(uint)", &MONO_CAMERA::GetAddress);
		mono_add_internal_call("CSScript.Camera::getactive(void*)", &MONO_CAMERA::GetActive);
		mono_add_internal_call("CSScript.Camera::setactive(void*,bool)", &MONO_CAMERA::SetActive);
	}
}