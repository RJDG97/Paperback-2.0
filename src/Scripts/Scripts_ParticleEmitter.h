#pragma once

#include "Mono.h"

namespace MONO_PARTICLEEMITTER
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_particleemitter = m_obj.m_pArchetype->FindComponent<particle_emitter>(m_obj.m_PoolDetails);

		return m_particleemitter;
	}

	MONO_EXPORT float GetEmitterLifetime(void* address)
	{
		if (address)
			return reinterpret_cast<particle_emitter*>(address)->m_Lifetime;

		return {};
	}

	MONO_EXPORT void SetEmitterLifetime(void* address, float value)
	{
		if (address)
			reinterpret_cast<particle_emitter*>(address)->m_Lifetime = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.ParticleEmitter::getaddress(uint)", &MONO_PARTICLEEMITTER::GetAddress);
		mono_add_internal_call("CSScript.ParticleEmitter::GetEmitterLifetime(void*)", &MONO_PARTICLEEMITTER::GetEmitterLifetime);
		mono_add_internal_call("CSScript.ParticleEmitter::SetEmitterLifetime(void*,single)", &MONO_PARTICLEEMITTER::SetEmitterLifetime);
	}
}