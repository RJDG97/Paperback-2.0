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

	MONO_EXPORT MonoString* GetParticleTexture(void* address)
	{
		if (address)
			return mono_string_new(mono_domain_get(), reinterpret_cast<particle_emitter*>(address)->m_TextureName.c_str());

		return {};
	}

	MONO_EXPORT void SetParticleTexture(void* address, MonoString* texture)
	{
		if (address)
			reinterpret_cast<particle_emitter*>(address)->m_TextureName = mono_string_to_utf8(texture);
	}

	MONO_EXPORT int GetEmissionRate(void* address)
	{
		if (address)
			return reinterpret_cast<particle_emitter*>(address)->m_EmissionRate;

		return {};
	}

	MONO_EXPORT void SetEmissionRate(void* address, int value)
	{
		if (address)
			reinterpret_cast<particle_emitter*>(address)->m_EmissionRate = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.ParticleEmitter::getaddress(uint)", &MONO_PARTICLEEMITTER::GetAddress);
		mono_add_internal_call("CSScript.ParticleEmitter::GetEmitterLifetime(void*)", &MONO_PARTICLEEMITTER::GetEmitterLifetime);
		mono_add_internal_call("CSScript.ParticleEmitter::SetEmitterLifetime(void*,single)", &MONO_PARTICLEEMITTER::SetEmitterLifetime);
		mono_add_internal_call("CSScript.ParticleEmitter::GetParticleTexture(void*)", &MONO_PARTICLEEMITTER::GetParticleTexture);
		mono_add_internal_call("CSScript.ParticleEmitter::SetParticleTexture(void*,string)", &MONO_PARTICLEEMITTER::SetParticleTexture);
		mono_add_internal_call("CSScript.ParticleEmitter::GetEmissionRate(void*)", &MONO_PARTICLEEMITTER::GetEmissionRate);
		mono_add_internal_call("CSScript.ParticleEmitter::SetEmissionRate(void*,int)", &MONO_PARTICLEEMITTER::SetEmissionRate);
	}
}