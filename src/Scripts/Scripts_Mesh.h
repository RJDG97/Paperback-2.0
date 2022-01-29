#pragma once

#include "Mono.h"

namespace MONO_MESH
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_mesh = m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_mesh)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Mesh component." << std::endl;
		}
#endif

		return m_mesh;
	}

	MONO_EXPORT MonoString* GetModel(void* address)
	{
		if (address)
			return mono_string_new(mono_domain_get(), reinterpret_cast<mesh*>(address)->m_Model.c_str());

		return {};
	}

	MONO_EXPORT MonoString* GetTexture(void* address)
	{
		if (address)
			return mono_string_new(mono_domain_get(), reinterpret_cast<mesh*>(address)->m_Texture.c_str());

		return {};
	}

	MONO_EXPORT int GetBias(void* address)
	{
		if (address)
			return reinterpret_cast<mesh*>(address)->m_Bias;

		return {};
	}

	MONO_EXPORT bool GetActive(void* address)
	{
		if (address)
			return reinterpret_cast<mesh*>(address)->m_Active;

		return {};
	}

	MONO_EXPORT void SetModel(void* address, MonoString* model)
	{
		if (address)
			reinterpret_cast<mesh*>(address)->m_Model = mono_string_to_utf8(model);
	}

	MONO_EXPORT void SetTexture(void* address, MonoString* texture)
	{
		if (address)
			reinterpret_cast<mesh*>(address)->m_Texture = mono_string_to_utf8(texture);
	}

	MONO_EXPORT void SetBias(void* address, int bias)
	{
		if (address)
			reinterpret_cast<mesh*>(address)->m_Bias = bias;
	}

	MONO_EXPORT void SetActive(void* address, bool active)
	{
		if (address)
			reinterpret_cast<mesh*>(address)->m_Active = active;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Mesh::getaddress(uint)", &MONO_MESH::GetAddress);
		mono_add_internal_call("CSScript.Mesh::getmodel(void*)", &MONO_MESH::GetModel);
		mono_add_internal_call("CSScript.Mesh::gettexture(void*)", &MONO_MESH::GetTexture);
		mono_add_internal_call("CSScript.Mesh::getbias(void*)", &MONO_MESH::GetBias);
		mono_add_internal_call("CSScript.Mesh::getactive(void*)", &MONO_MESH::GetActive);
		mono_add_internal_call("CSScript.Mesh::setmodel(void*,string)", &MONO_MESH::SetModel);
		mono_add_internal_call("CSScript.Mesh::settexture(void*,string)", &MONO_MESH::SetTexture);
		mono_add_internal_call("CSScript.Mesh::setbias(void*,int)", &MONO_MESH::SetBias);
		mono_add_internal_call("CSScript.Mesh::setactive(void*,bool)", &MONO_MESH::SetActive);
	}
}