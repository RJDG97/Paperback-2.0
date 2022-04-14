/**********************************************************************************
*\file         Scripts_Mesh.h
*\brief        Scripts_Mesh.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

	MONO_EXPORT MonoString* GetModel(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return mono_string_new(mono_domain_get(), m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails)->m_Model.c_str());

		return {};
	}

	MONO_EXPORT MonoString* GetTexture(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return mono_string_new(mono_domain_get(), m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails)->m_Texture.c_str());

		return {};
	}

	MONO_EXPORT int GetBias(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails)->m_Bias;

		return {};
	}

	MONO_EXPORT bool GetActive(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails)->m_Active;

		return {};
	}

	MONO_EXPORT void SetModel(uint32_t ID, MonoString* model)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails)->m_Model = mono_string_to_utf8(model);
	}

	MONO_EXPORT void SetTexture(uint32_t ID, MonoString* texture)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails)->m_Texture = mono_string_to_utf8(texture);
	}

	MONO_EXPORT void SetBias(uint32_t ID, int bias)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails)->m_Bias = bias;
	}

	MONO_EXPORT void SetActive(uint32_t ID, bool active)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<mesh>(m_obj.m_PoolDetails)->m_Active = active;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Mesh::getaddress(uint)", &MONO_MESH::GetAddress);
		mono_add_internal_call("CSScript.Mesh::getmodel(uint)", &MONO_MESH::GetModel);
		mono_add_internal_call("CSScript.Mesh::gettexture(uint)", &MONO_MESH::GetTexture);
		mono_add_internal_call("CSScript.Mesh::getbias(uint)", &MONO_MESH::GetBias);
		mono_add_internal_call("CSScript.Mesh::getactive(uint)", &MONO_MESH::GetActive);
		mono_add_internal_call("CSScript.Mesh::setmodel(uint,string)", &MONO_MESH::SetModel);
		mono_add_internal_call("CSScript.Mesh::settexture(uint,string)", &MONO_MESH::SetTexture);
		mono_add_internal_call("CSScript.Mesh::setbias(uint,int)", &MONO_MESH::SetBias);
		mono_add_internal_call("CSScript.Mesh::setactive(uint,bool)", &MONO_MESH::SetActive);
	}
}