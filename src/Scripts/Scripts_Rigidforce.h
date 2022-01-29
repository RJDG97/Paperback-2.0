#pragma once

#include "Mono.h"

namespace MONO_RIGIDFORCE
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_rigidforce = m_obj.m_pArchetype->FindComponent<rigidforce>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_rigidforce)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Rigidforce component." << std::endl;
		}
#endif

		return m_rigidforce;
	}

	MONO_EXPORT float GetStaticFriction(void* address)
	{
		if (address)
			return reinterpret_cast<rigidforce*>(address)->m_staticFriction;

		return {};
	}

	MONO_EXPORT void SetStaticFriction(void* address, float value)
	{
		if (address)
			reinterpret_cast<rigidforce*>(address)->m_staticFriction = value;
	}

	MONO_EXPORT float GetDynamicFriction(void* address)
	{
		if (address)
			return reinterpret_cast<rigidforce*>(address)->m_dynamicFriction;

		return {};
	}

	MONO_EXPORT void SetDynamicFriction(void* address, float value)
	{
		if (address)
			reinterpret_cast<rigidforce*>(address)->m_dynamicFriction = value;
	}

	MONO_EXPORT paperback::Vector3f GetForces(void* address)
	{
		if (address)
			return reinterpret_cast<rigidforce*>(address)->m_Forces;

		return {};
	}

	MONO_EXPORT void SetForces(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<rigidforce*>(address)->m_Forces = { x, y, z };
	}

	MONO_EXPORT paperback::Vector3f GetMomentum(void* address)
	{
		if (address)
			return reinterpret_cast<rigidforce*>(address)->m_Momentum;

		return {};
	}

	MONO_EXPORT void SetMomentum(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<rigidforce*>(address)->m_Momentum = { x, y, z };
	}

	MONO_EXPORT float GetRestitution(void* address)
	{
		if (address)
			return reinterpret_cast<rigidforce*>(address)->m_Restitution;

		return {};
	}

	MONO_EXPORT void SetRestitution(void* address, float value)
	{
		if (address)
			reinterpret_cast<rigidforce*>(address)->m_Restitution = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Rigidforce::getaddress(uint)", &MONO_RIGIDFORCE::GetAddress);
		mono_add_internal_call("CSScript.Rigidforce::getstaticfriction(void*)", &MONO_RIGIDFORCE::GetStaticFriction);
		mono_add_internal_call("CSScript.Rigidforce::setstaticfriction(void*,single)", &MONO_RIGIDFORCE::SetStaticFriction);
		mono_add_internal_call("CSScript.Rigidforce::getdynamicfriction(void*)", &MONO_RIGIDFORCE::GetDynamicFriction);
		mono_add_internal_call("CSScript.Rigidforce::setdynamicfriction(void*,single)", &MONO_RIGIDFORCE::SetDynamicFriction);
		mono_add_internal_call("CSScript.Rigidforce::getforces(void*)", &MONO_RIGIDFORCE::GetForces);
		mono_add_internal_call("CSScript.Rigidforce::setforces(void*,single,single,single)", &MONO_RIGIDFORCE::SetForces);
		mono_add_internal_call("CSScript.Rigidforce::getmomentum(void*)", &MONO_RIGIDFORCE::GetMomentum);
		mono_add_internal_call("CSScript.Rigidforce::setmomentum(void*,single,single,single)", &MONO_RIGIDFORCE::SetMomentum);
		mono_add_internal_call("CSScript.Rigidforce::getrestitution(void*)", &MONO_RIGIDFORCE::GetRestitution);
		mono_add_internal_call("CSScript.Rigidforce::setrestitution(void*,single)", &MONO_RIGIDFORCE::SetRestitution);
	}
}