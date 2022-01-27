#pragma once

#include "Mono.h"

namespace MONO_COLLISIONLAYER
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_collidable = m_obj.m_pArchetype->FindComponent<collidable>(m_obj.m_PoolDetails);
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