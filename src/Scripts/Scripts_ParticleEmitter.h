/**********************************************************************************
*\file         Scripts_ParticleEmitter.h
*\brief        Scripts_ParticleEmitter.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

	MONO_EXPORT float GetEmitterLifetime(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<particle_emitter>(m_obj.m_PoolDetails)->m_Lifetime;

		return {};
	}

	MONO_EXPORT void SetEmitterLifetime(uint32_t ID, float value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<particle_emitter>(m_obj.m_PoolDetails)->m_Lifetime = value;
	}

	MONO_EXPORT MonoString* GetParticleTexture(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return mono_string_new(mono_domain_get(), m_obj.m_pArchetype->FindComponent<particle_emitter>(m_obj.m_PoolDetails)->m_TextureName.c_str());

		return {};
	}

	MONO_EXPORT void SetParticleTexture(uint32_t ID, MonoString* texture)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<particle_emitter>(m_obj.m_PoolDetails)->m_TextureName = mono_string_to_utf8(texture);
	}

	MONO_EXPORT int GetEmissionRate(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<particle_emitter>(m_obj.m_PoolDetails)->m_EmissionRate;

		return {};
	}

	MONO_EXPORT void SetEmissionRate(uint32_t ID, int value)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		m_obj.m_pArchetype->FindComponent<particle_emitter>(m_obj.m_PoolDetails)->m_EmissionRate = value;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.ParticleEmitter::getaddress(uint)", &MONO_PARTICLEEMITTER::GetAddress);
		mono_add_internal_call("CSScript.ParticleEmitter::GetEmitterLifetime(uint)", &MONO_PARTICLEEMITTER::GetEmitterLifetime);
		mono_add_internal_call("CSScript.ParticleEmitter::SetEmitterLifetime(uint,single)", &MONO_PARTICLEEMITTER::SetEmitterLifetime);
		mono_add_internal_call("CSScript.ParticleEmitter::GetParticleTexture(uint)", &MONO_PARTICLEEMITTER::GetParticleTexture);
		mono_add_internal_call("CSScript.ParticleEmitter::SetParticleTexture(uint,string)", &MONO_PARTICLEEMITTER::SetParticleTexture);
		mono_add_internal_call("CSScript.ParticleEmitter::GetEmissionRate(uint)", &MONO_PARTICLEEMITTER::GetEmissionRate);
		mono_add_internal_call("CSScript.ParticleEmitter::SetEmissionRate(uint,int)", &MONO_PARTICLEEMITTER::SetEmissionRate);
	}
}