#pragma once

#include "Mono.h"

namespace MONO_MESH
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_mesh = m_obj.m_pArchetype->GetComponent<mesh>(m_obj.m_PoolDetails);
		return &m_mesh;
	}

	MONO_EXPORT std::string GetModel(void* address)
	{
		if (address)
			return reinterpret_cast<mesh*>(address)->m_Model;

		return {};
	}

	MONO_EXPORT std::string GetTexture(void* address)
	{
		if (address)
			return reinterpret_cast<mesh*>(address)->m_Texture;

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

	MONO_EXPORT void SetModel(void* address, std::string model)
	{
		if (address)
			reinterpret_cast<mesh*>(address)->m_Model = model;
	}

	MONO_EXPORT void SetTexture(void* address, std::string texture)
	{
		if (address)
			reinterpret_cast<mesh*>(address)->m_Texture = texture;
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