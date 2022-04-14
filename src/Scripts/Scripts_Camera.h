/**********************************************************************************
*\file         Scripts_Camera.h
*\brief        Scripts_Camera.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

	MONO_EXPORT bool GetActive(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<camera>(m_obj.m_PoolDetails)->m_Active;

		return {};
	}

	MONO_EXPORT void SetActive(uint32_t ID, bool value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<camera>(m_obj.m_PoolDetails)->m_Active = value;
	}

	MONO_EXPORT paperback::Vector3f GetPos(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			glm::vec3 pos{ m_obj.m_pArchetype->FindComponent<camera>(m_obj.m_PoolDetails)->m_Position };
			return { pos.x, pos.y, pos.z };
	}

	MONO_EXPORT void SetPos(uint32_t ID, float x, float y, float z)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<camera>(m_obj.m_PoolDetails)->m_Position = {x, y, z};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Camera::getaddress(uint)", &MONO_CAMERA::GetAddress);
		mono_add_internal_call("CSScript.Camera::getactive(uint)", &MONO_CAMERA::GetActive);
		mono_add_internal_call("CSScript.Camera::setactive(uint,bool)", &MONO_CAMERA::SetActive);
		mono_add_internal_call("CSScript.Camera::getpos(uint)", &MONO_CAMERA::GetPos);
		mono_add_internal_call("CSScript.Camera::setpos(uint,single,single,single)", &MONO_CAMERA::SetPos);
	}
}