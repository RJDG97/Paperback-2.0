#pragma once

#include "Mono.h"

namespace MONO_SPHERE
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_sphere = m_obj.m_pArchetype->FindComponent<sphere>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_sphere)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Sphere component." << std::endl;
		}
#endif

		return m_sphere;
	}

	MONO_EXPORT float GetRadius(void* address)
	{
		if (address)
			return reinterpret_cast<sphere*>(address)->m_Radius;

		return {};
	}

	MONO_EXPORT bool GetCollided(void* address)
	{
		if (address)
			return reinterpret_cast<sphere*>(address)->m_Collided;

		return {};
	}

	MONO_EXPORT void SetRadius(void* address, float value)
	{
		if (address)
			reinterpret_cast<sphere*>(address)->m_Radius = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Sphere::getaddress(uint)", &MONO_SPHERE::GetAddress);
		mono_add_internal_call("CSScript.Sphere::getradius(void*)", &MONO_SPHERE::GetRadius);
		mono_add_internal_call("CSScript.Sphere::setradius(void*,single)", &MONO_SPHERE::SetRadius);
		mono_add_internal_call("CSScript.Sphere::getcollided(void*)", &MONO_SPHERE::GetCollided);
	}
}