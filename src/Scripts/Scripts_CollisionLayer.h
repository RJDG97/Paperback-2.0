/**********************************************************************************
*\file         Scripts_CollisionLayer.h
*\brief        Scripts_CollisionLayer.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_COLLISIONLAYER
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_collidable = m_obj.m_pArchetype->FindComponent<collidable>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_collidable)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Collision Layer / Collidable component." << std::endl;
		}
#endif

		return m_collidable;
	}

	MONO_EXPORT uint8_t GetCollisionLayer(void* address)
	{
		if (address)
			return reinterpret_cast<collidable*>(address)->m_CollisionLayer;

		return {};
	}

	MONO_EXPORT void SetCollisionLayer(void* address, uint8_t value)
	{
		if (address)
			reinterpret_cast<collidable*>(address)->Set(value);
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.CollisionLayer::getaddress(uint)", &MONO_COLLISIONLAYER::GetAddress);
		mono_add_internal_call("CSScript.CollisionLayer::getcollisionlayer(void*)", &MONO_COLLISIONLAYER::GetCollisionLayer);
		mono_add_internal_call("CSScript.CollisionLayer::setcollisionlayer(void*,uint)", &MONO_COLLISIONLAYER::SetCollisionLayer);
	}
}