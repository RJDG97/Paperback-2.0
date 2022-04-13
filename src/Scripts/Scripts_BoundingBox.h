/**********************************************************************************
*\file         Scripts_BoundingBox.h
*\brief        Scripts_BoundingBox.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_BOUNDINGBOX
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_boundingbox = m_obj.m_pArchetype->FindComponent<boundingbox>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_boundingbox)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Bounding Box component." << std::endl;
		}
#endif

		return m_boundingbox;
	}

	MONO_EXPORT paperback::Vector3f GetMin(void* address)
	{
		if (address)
			return reinterpret_cast<boundingbox*>(address)->Min;

		return {};
	}

	MONO_EXPORT void SetMin(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<boundingbox*>(address)->Min = {x, y, z};
	}

	MONO_EXPORT paperback::Vector3f GetMax(void* address)
	{
		if (address)
			return reinterpret_cast<boundingbox*>(address)->Max;

		return {};
	}

	MONO_EXPORT void SetMax(void* address, float x, float y, float z)
	{
		if (address)
			reinterpret_cast<boundingbox*>(address)->Max = { x, y, z };
	}

	MONO_EXPORT bool GetCollided(void* address)
	{
		if (address)
			return reinterpret_cast<boundingbox*>(address)->m_Collided;

		return {};
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.BoundingBox::getaddress(uint)", &MONO_BOUNDINGBOX::GetAddress);
		mono_add_internal_call("CSScript.BoundingBox::getmin(void*)", &MONO_BOUNDINGBOX::GetMin);
		mono_add_internal_call("CSScript.BoundingBox::setmin(void*,single,single,single)", &MONO_BOUNDINGBOX::SetMin);
		mono_add_internal_call("CSScript.BoundingBox::getmax(void*)", &MONO_BOUNDINGBOX::GetMax);
		mono_add_internal_call("CSScript.BoundingBox::setmax(void*,single,single,single)", &MONO_BOUNDINGBOX::SetMax);
		mono_add_internal_call("CSScript.BoundingBox::getcollided(void*)", &MONO_BOUNDINGBOX::GetCollided);
	}
}